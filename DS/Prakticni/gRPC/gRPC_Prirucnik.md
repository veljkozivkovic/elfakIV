# gRPC Priručnik

---

## Pregled koraka

```
1. Definiši poruke i servis  →  .proto fajl
2. Implementiraj logiku      →  Services/XxxService.cs
3. Registruj servis          →  Server Program.cs
4. Pozovi servis             →  Klijent Program.cs
```

Svaki gRPC projekat uvek ide ovim redosledom. Proto fajl je "ugovor" između klijenta i servera — sve ostalo se gradi oko njega.

---

## KORAK 1 — `.proto` fajl

### Zašto postoji

`.proto` fajl je **centralna definicija svega** — koje poruke postoje, koja polja imaju, i koje metode servis nudi. Iz njega se **automatski generiše C# kod** (klase za poruke, bazne klase za server, stub klase za klijent). Bez njega nema ničega.

Ovaj fajl je **identičan na serveru i klijentu** osim jedne linije — `csharp_namespace`.

### Kako se piše — osnovna struktura

```protobuf
syntax = "proto3";
// Obavezno na prvom mestu. Govori kompajleru koja verzija sintakse se koristi.
// proto3 = nema "required" polja, sve je optional po defaultu.

import "google/protobuf/empty.proto";
// Pišeš ovo SAMO ako neka metoda ne treba da vrati ništa (kao void).
// Empty je Googleov ugrađeni tip koji predstavlja prazan odgovor.

option csharp_namespace = "ImeServerskogNamespacea";
// Govori protoc kompajleru u koji C# namespace da smesti generisane klase.
// NA KLIJENTU se menja u "ImeKlijentskogNamespacea" — to je jedina razlika!

package ime_paketa;
// Logičko ime paketa u protobuf sistemu.
// Služi da izbegneš konflikt naziva ako imaš više proto fajlova.

service ImeServisa {
    // Ovde definišeš RPC metode (vidi dole po tipovima)
}

message ImePoruke {
    // Ovde definišeš strukturu podataka
    tip_polja ime_polja = tag;
}
```

### Kako se definiše poruka (`message`)

```protobuf
message Student {
    int32  brojIndeksa = 1;
    // int32  = 32-bitni ceo broj
    // brojIndeksa = ime polja (u C# postaje BrojIndeksa — PascalCase)
    // = 1 = TAG — jedinstven broj koji identifikuje polje u binarnom formatu.
    //   Nikad ga ne menjaj nakon što pushuješ na produkciju!
    //   Počni od 1 i idi redom. Nema veze sa vrednošću polja.

    string imePrezime = 2;
    // string = tekstualni tip
}
```

Dostupni tipovi podataka:

| Proto tip | C# tip | Kada koristiti |
|-----------|--------|----------------|
| `int32` | `int` | Celi brojevi |
| `int64` | `long` | Veliki celi brojevi |
| `string` | `string` | Tekst |
| `bool` | `bool` | Tačno/netačno |
| `float` | `float` | Decimalni brojevi |
| `double` | `double` | Precizni decimalni |
| `bytes` | `ByteString` | Binarni podaci |
| `repeated TipX` | `List<TipX>` | Lista/niz vrednosti |
| `google.protobuf.Empty` | `Empty` | Bez povratne vrednosti |
| Druga `message` | Generisana klasa | Ugneždena struktura |

### Kako se definiše servis — 4 tipa metoda

```protobuf
service ImeServisa {

    // 1. UNARY — jedan zahtev, jedan odgovor (kao obična funkcija)
    rpc ImeMetode (UlazniTip) returns (IzlazniTip);

    // 2. CLIENT STREAMING — klijent šalje više poruka, server vraća jednu
    rpc ImeMetode (stream UlazniTip) returns (IzlazniTip);
    // "stream" ispred ulaznog tipa = klijent šalje tok

    // 3. SERVER STREAMING — klijent šalje jednu, server vraća više
    rpc ImeMetode (UlazniTip) returns (stream IzlazniTip);
    // "stream" ispred izlaznog tipa = server vraća tok

    // 4. BIDIRECTIONAL STREAMING — obe strane šalju tok istovremeno
    rpc ImeMetode (stream UlazniTip) returns (stream IzlazniTip);
    // "stream" na oba mesta

    // Ako metoda ne vraća ništa (void):
    rpc ObrisiNesto (UlazniTip) returns (google.protobuf.Empty);
    // Zahteva: import "google/protobuf/empty.proto"; na vrhu fajla
}
```

### Primer kompletnog proto fajla

```protobuf
syntax = "proto3";
import "google/protobuf/empty.proto";

option csharp_namespace = "MojServis";
package moj;

service MojServis {
    rpc Unary              (Podatak) returns (Podatak);
    rpc KlijentStreaming    (stream Podatak) returns (Podatak);
    rpc ServerStreaming     (Podatak) returns (stream Podatak);
    rpc BidiStreaming       (stream Podatak) returns (stream Podatak);
    rpc BrisanjeBezOdg     (Podatak) returns (google.protobuf.Empty);
}

message Podatak {
    int32  vrednost   = 1;
    string naziv      = 2;
}
```

### Razlika između server i klijent proto fajla

Fajl je potpuno isti osim jedne linije:

```protobuf
// SERVER proto:
option csharp_namespace = "Elfak";

// KLIJENT proto (isti fajl, samo ova linija drugačija):
option csharp_namespace = "ElfakClient";
```

Zašto? Zato što generisane klase moraju biti u namespace-u projekta u kom se nalaze. Ako bi oba koristila isti namespace, došlo bi do konflikta pri kompajliranju.

---

## KORAK 2 — `Services/XxxService.cs` (Server)

### Zašto postoji

Ovo je fajl gde pišeš **stvarnu logiku** — šta server radi kad primi zahtev. Klasa **nasleđuje** automatski generisanu baznu klasu i **override-uje** metode koje hoćeš da implementiraš.

Bazna klasa se automatski generiše iz proto fajla kada buildujemo projekat. Ako u proto fajlu definišeš servis `MojServis`, generiše se klasa `MojServis.MojServisBase` — nju nasleđuješ.

### Osnovna struktura klase

```csharp
using Google.Protobuf.WellKnownTypes; // obavezno ako koristiš Empty
using Grpc.Core;                       // obavezno uvek — ServerCallContext

namespace MojProjekt.Services
{
    public class MojServisService : MojServis.MojServisBase
    // "MojServis.MojServisBase" = generisana bazna klasa iz proto fajla
    // Uvek je format: ImeServisa.ImeServisaBase
    {
        // Override metode ovde
    }
}
```

### Implementacija po tipu metode

---

#### UNARY — jedan zahtev, jedan odgovor

```csharp
public override Task<IzlazniTip> ImeMetode(UlazniTip request, ServerCallContext context)
// Task<IzlazniTip> = asinhroni zadatak koji vraća jednu poruku
// UlazniTip request = poruka primljena od klijenta (automatski deserijalizovana)
// ServerCallContext context = info o pozivu: metapodaci, deadline, token za otkazivanje
{
    // Pristupaš poljima request objekta kao regularnim C# property-jima
    // Polja iz proto fajla: brojIndeksa = 1 → u C# postaje request.BrojIndeksa

    var rezultat = new IzlazniTip
    {
        NekoPoljе = request.NekoPoljе
        // Popunjavaš output objekat
    };

    return Task.FromResult(rezultat);
    // Task.FromResult() = pravi već-završen Task bez čekanja
    // Koristiš ovo kada nema stvarnog asinhronog posla (nema await operacija)
}

// Ako koristiš async/await unutra:
public override async Task<IzlazniTip> ImeMetode(UlazniTip request, ServerCallContext context)
{
    var podatak = await nekiAsyncPoziv();
    return await Task.FromResult(new IzlazniTip { Polje = podatak });
}

// Ako metoda vraća Empty (ništa):
public override Task<Empty> ImeMetode(UlazniTip request, ServerCallContext context)
{
    // uradi nešto
    return Task.FromResult(new Empty());
    // Empty() = prazan objekat, kao void
}
```

---

#### CLIENT STREAMING — klijent šalje tok, server vraća jednu poruku

```csharp
public override async Task<IzlazniTip> ImeMetode(
    IAsyncStreamReader<UlazniTip> requestStream,
    // IAsyncStreamReader = interfejs za čitanje toka poruka jednu po jednu
    ServerCallContext context)
// Nema IServerStreamWriter jer server vraća JEDNU poruku na kraju, ne tok
{
    // Čitaš sve poruke koje klijent šalje:
    await foreach (var poruka in requestStream.ReadAllAsync())
    // await foreach = asinhrono čekamo sledeću poruku
    // .ReadAllAsync() = vraća IAsyncEnumerable — iterira dok klijent ne završi
    // Petlja se završava automatski kad klijent pozove CompleteAsync()
    {
        // obradi svaku poruku
        // npr. sabiranje, prikupljanje u listu, čuvanje u bazu...
    }

    // Nakon što klijent završi, šalješ jednu poruku kao odgovor:
    return await Task.FromResult(new IzlazniTip { /* rezultat */ });
}

// Primer — sabiranje brojeva:
public override async Task<Number> ClientStreaming(
    IAsyncStreamReader<Number> requestStream, ServerCallContext context)
{
    int zbir = 0;
    await foreach (var broj in requestStream.ReadAllAsync())
    {
        zbir += broj.Value;
    }
    return await Task.FromResult(new Number { Value = zbir });
}
```

---

#### SERVER STREAMING — klijent šalje jednu, server vraća tok

```csharp
public override async Task ImeMetode(
    UlazniTip request,
    // Klijent šalje jednu poruku — normalan request parametar
    IServerStreamWriter<IzlazniTip> responseStream,
    // IServerStreamWriter = interfejs za pisanje poruka ka klijentu
    ServerCallContext context)
// Povratni tip je Task (ne Task<T>) jer ne vraćaš jednu vrednost direktno
{
    // Šalješ više poruka u petlji:
    for (int i = 0; i < request.Vrednost; i++)
    {
        await responseStream.WriteAsync(new IzlazniTip { Polje = i });
        // await = čekaš da se poruka zapiše u tok pre nastavka
        // Klijent prima svaku poruku čim je WriteAsync završi
    }
    // Kad metoda završi (return/kraj), server automatski zatvara tok
}

// Primer — slanje silaznog niza:
public override async Task ServerStreaming(
    Number request, IServerStreamWriter<Number> responseStream, ServerCallContext context)
{
    for (int i = request.Value; i > 0; i--)
    {
        await responseStream.WriteAsync(new Number { Value = i });
    }
}
```

---

#### BIDIRECTIONAL STREAMING — obe strane šalju tok istovremeno

```csharp
public override async Task ImeMetode(
    IAsyncStreamReader<UlazniTip> requestStream,
    // Čitaš tok od klijenta
    IServerStreamWriter<IzlazniTip> responseStream,
    // Pišeš tok ka klijentu
    ServerCallContext context)
// Oba parametra postoje — čitaš I pišeš
{
    await foreach (var poruka in requestStream.ReadAllAsync())
    // Iteriraš kroz sve poruke klijenta
    {
        // Za svaku primljenu poruku odmah odgovaraš:
        await responseStream.WriteAsync(new IzlazniTip
        {
            Polje = obradiPoruku(poruka)
        });
        // Ovo je "echo" šablon — primi → odmah odgovori
        // Tokovi su NEZAVISNI: ne moraš da odgovaraš na svaku poruku,
        // možeš odgovoriti jednom na kraju, ili više puta na jednu poruku
    }
}

// Primer — echo sa uvećanjem za 1:
public override async Task BidiStreaming(
    IAsyncStreamReader<Number> requestStream,
    IServerStreamWriter<Number> responseStream,
    ServerCallContext context)
{
    await foreach (var broj in requestStream.ReadAllAsync())
    {
        await responseStream.WriteAsync(new Number { Value = broj.Value + 1 });
    }
}
```

---

### Pristup zajedničkim podacima na serveru

Kada više klijenata poziva server, svi dele iste podatke (npr. istu bazu). Postoje dva pristupa:

**Opcija A — Static field (jednostavno, nije thread-safe)**
```csharp
public class MojService : MojServis.MojServisBase
{
    private static List<string> podaci = new List<string>();
    // static = jedna lista za sve instance servisa
    // Problem: nije thread-safe — dva klijenta istovremeno mogu da prave probleme
}
```

**Opcija B — Singleton pattern (preporučeno, thread-safe)**

Pravi se posebna klasa za podatke (vidi sekciju Design Patterns).

---

## KORAK 3 — Server `Program.cs`

### Zašto postoji

Ovo je ulazna tačka servera. Ovde se kreira ASP.NET Core aplikacija, registruje gRPC podrška i registruju svi servisi tako da budu dostupni klijentima.

### Kako se piše

```csharp
using MojProjekt.Services;
// Uvozimo namespace gde su naši servisni fajlovi

var builder = WebApplication.CreateBuilder(args);
// Kreira se "builder" — konfigurišemo šta će aplikacija imati
// args = argumenti prosleđeni iz komandne linije (retko relevantno)

builder.Services.AddGrpc();
// OBAVEZNO — registruje gRPC u Dependency Injection kontejner
// Bez ove linije gRPC neće raditi uopšte

var app = builder.Build();
// Gradi se konačna aplikacija sa svim registrovanim servisima
// Posle Build() ne možeš više da dodaješ servise

// Registrujemo svaki servis koji hoćemo da klijenti mogu da pozivaju:
app.MapGrpcService<PrviServis>();
app.MapGrpcService<DrugiServis>();
// Možeš registrovati koliko god servisa hoćeš
// Redosled nije važan

app.MapGet("/", () => "gRPC server radi. Koristite gRPC klijent za pozive.");
// Ova linija nije obavezna, ali korisna — ako neko otvori browser,
// vidi poruku umesto greške. gRPC protokol nije kompatibilan sa browserom.

app.Run();
// Pokreće server — blokira i sluša zahteve beskonačno dok se ne ugasi
```

### Najčešće greške u Program.cs

```csharp
// GREŠKA — zaboravljen AddGrpc():
var builder = WebApplication.CreateBuilder(args);
// builder.Services.AddGrpc();  ← ako izostavljaš ovo, MapGrpcService baca grešku
var app = builder.Build();
app.MapGrpcService<MojServis>(); // ERROR!

// GREŠKA — pogrešan namespace u using:
using NePostojeciNamespace.Services; // ako namespace ne odgovara folderu/klasi
```

---

## KORAK 4 — Klijent `Program.cs`

### Zašto postoji

Klijent je program koji se **spaja na server** i poziva metode kao da su lokalne funkcije. Koristi generisanu stub klasu iz proto fajla.

### Osnovna inicijalizacija

```csharp
using Grpc.Net.Client;
using ImeKlijentskogNamespacea; // namespace iz option csharp_namespace u proto fajlu

using var channel = GrpcChannel.ForAddress("http://localhost:5000");
// GrpcChannel = konekcija ka serveru
// ForAddress = adresa servera (http za development, https za produkciju)
// Port mora odgovarati onome na čemu server sluša (vidi launchSettings.json servera)
// "using var" = kanal se automatski zatvara kad program završi (IDisposable)

var client = new ImeServisa.ImeServisaClient(channel);
// Pravi se stub (klijentski objekat) koristeći kanal
// ImeServisa.ImeServisaClient je automatski generisana klasa iz proto fajla
// Format uvek: ImeServisa.ImeServisaClient
// Kroz ovaj objekat pozivamo sve metode servera
```

### Pozivi po tipu metode

---

#### UNARY — poziv

```csharp
// Async verzija (preporučeno):
var odgovor = await client.ImeMetodeAsync(new UlazniTip
{
    Polje = vrednost
});
// await = čekamo odgovor servera pre nastavka
// ImeMetodeAsync = automatski generisana async verzija (uvek postoji)
Console.WriteLine(odgovor.Polje);

// Sync verzija (blokira nit):
var odgovor = client.ImeMetode(new UlazniTip { Polje = vrednost });
// Bez Async sufiksa, bez await — blokira trenutnu nit dok ne dobije odgovor
// Koristi se retko, uglavnom u testovima ili jednostavnim konzolnim programima
```

---

#### CLIENT STREAMING — poziv

```csharp
var poziv = client.ImeMetode();
// Otvaramo streaming poziv — ne šalje ništa još, samo otvara kanal
// Nema "new UlazniTip" ovde jer šaljemo više poruka posle

// Šaljemo poruke jednu po jednu:
await poziv.RequestStream.WriteAsync(new UlazniTip { Polje = vrednost1 });
await poziv.RequestStream.WriteAsync(new UlazniTip { Polje = vrednost2 });
// WriteAsync = šalje jednu poruku u tok
// await = čekamo da se poruka zapiše pre nego što šaljemo sledeću

await poziv.RequestStream.CompleteAsync();
// OBAVEZNO — govorimo serveru "više nemamo poruka"
// Bez ovoga server nikad neće znati da završi i čekaće zauvek

var odgovor = await poziv;
// Tek sada čekamo odgovor servera (server odgovara jednom, nakon CompleteAsync)
Console.WriteLine(odgovor.Polje);

// Ili u petlji:
var poziv = client.ImeMetode();
for (int i = 0; i < 10; i++)
{
    await poziv.RequestStream.WriteAsync(new UlazniTip { Vrednost = i });
}
await poziv.RequestStream.CompleteAsync();
var rezultat = await poziv;
```

---

#### SERVER STREAMING — poziv

```csharp
var poziv = client.ImeMetode(new UlazniTip { Polje = vrednost });
// Šaljemo jednu poruku — server počinje da šalje tok nazad

// Čitamo sve poruke koje server šalje:
await foreach (var poruka in poziv.ResponseStream.ReadAllAsync())
// await foreach = čekamo svaku poruku čim stigne
// Petlja završava kad server zatvori tok (kad njegova metoda završi)
{
    Console.WriteLine(poruka.Polje);
    // Obradiš svaku poruku odmah — ne čekaš sve
}
```

---

#### BIDIRECTIONAL STREAMING — poziv

```csharp
var poziv = client.ImeMetode();
// Otvaramo bidirectional poziv — oba toka su otvorena

// KLJUČNO: čitanje i pisanje se MORA raditi ISTOVREMENO u posebnim taskovima
// Ako pokušaš da pišeš i čitaš sekvencijalno, program se može zaključati!

// Pozadinski task koji čita odgovore dok mi šaljemo:
var pozadinskiTask = Task.Run(async () =>
{
    await foreach (var odgovor in poziv.ResponseStream.ReadAllAsync())
    {
        Console.WriteLine(odgovor.Polje);
    }
});
// Task.Run() = pokreće kod u posebnoj niti iz thread pool-a
// Ovaj task ISTOVREMENO čita dok mi u glavnoj niti šaljemo

// U glavnoj niti šaljemo poruke:
await poziv.RequestStream.WriteAsync(new UlazniTip { Polje = vrednost1 });
await poziv.RequestStream.WriteAsync(new UlazniTip { Polje = vrednost2 });

await poziv.RequestStream.CompleteAsync();
// Govorimo serveru "završili smo sa slanjem"
// Server tada zatvara i response tok

await pozadinskiTask;
// Čekamo da pozadinski task pročita sve preostale odgovore pre kraja programa

// Thread.Sleep(1000) između WriteAsync-ova vizuelno pokazuje realtime streaming:
for (int i = 0; i < 10; i++)
{
    await poziv.RequestStream.WriteAsync(new UlazniTip { Polje = i });
    Thread.Sleep(1000); // čeka 1 sekundu — videćeš odgovore kako stižu
}
```

---

### Interaktivni meni (do/while i switch)

```csharp
string unos;
do
{
    Console.WriteLine("a - Dodaj");
    Console.WriteLine("b - Obrisi");
    Console.WriteLine("x - Izlaz");
    unos = Console.ReadLine();

    switch (unos)
    {
        case "a": await DodajNesto(); break;
        case "b": await ObrisiNesto(); break;
        case "x": break;
        default: Console.WriteLine("Nepoznata opcija"); break;
    }
} while (unos != "x");
// do/while = meni se uvek prikaže bar jednom
// Petlja traje dok korisnik ne unese "x"

// Metode se pišu ISPOD glavnog koda (top-level statements u C# 9+):
async Task DodajNesto()
{
    // logika...
}
```

---

## KORAK 5 — `.csproj` fajlovi (konfiguracija projekata)

### Server `.csproj`

```xml
<ItemGroup>
  <Protobuf Include="Protos/ime_fajla.proto" GrpcServices="Server" />
  <!-- GrpcServices="Server" = generiši samo serverske bazne klase -->
  <!-- Dodaj po jednu liniju za svaki .proto fajl -->
</ItemGroup>
```

### Klijent `.csproj`

```xml
<ItemGroup>
  <Protobuf Include="Protos/ime_fajla.proto" GrpcServices="Client" />
  <!-- GrpcServices="Client" = generiši samo klijentske stub klase -->
</ItemGroup>
```

### Potrebni NuGet paketi

**Server projekat:**
```
Grpc.AspNetCore    ← gRPC integracija sa ASP.NET Core
Grpc.Tools         ← protoc kompajler koji generiše klase
Google.Protobuf    ← biblioteka za protobuf serijalizaciju
```

**Klijent projekat:**
```
Grpc.Net.Client    ← klijentska biblioteka za gRPC
Grpc.Tools         ← protoc kompajler koji generiše klase
Google.Protobuf    ← biblioteka za protobuf serijalizaciju
```

---

## Design Patterns u gRPC projektima

---

### Pattern 1 — Singleton (najčešći)

#### Kada se koristi

Kada server treba da ima **jednu zajedničku instancu podataka** (npr. bazu u memoriji) koju dele svi klijenti i svi pozivi. Bez Singletona, svaki poziv bi pravio novu instancu sa praznim podacima.

#### Zašto nije samo `static`

Prosta static varijabla nije thread-safe — ako dva klijenta istovremeno pristupe podacima, može doći do trke (race condition). Singleton sa `lock` mehanizmom to sprečava.

#### Implementacija

```csharp
public class Baza
{
    // 1. Čuvamo jedinu instancu kao static field
    private static Baza instanca;

    // 2. Objekat za zaključavanje niti
    private static object lockObj = new object();

    // 3. Sami podaci (ovde Dictionary kao primer)
    public Dictionary<int, string> Podaci { get; set; }

    // 4. Privatni konstruktor — NIKO spolja ne može new Baza()
    private Baza()
    {
        Podaci = new Dictionary<int, string>
        {
            { 1, "Početni podatak" }
            // inicijalni podaci ako treba
        };
    }

    // 5. Jedini način da dođeš do instance
    public static Baza Instanca()
    {
        if (instanca == null)                  // Prva provera (bez zaključavanja — brzo)
        {
            lock (lockObj)                     // Zaključaj — samo jedna nit ulazi
            {
                if (instanca == null)          // Druga provera (unutar locka — sigurno)
                {
                    instanca = new Baza();     // Napravi instancu jednom zauvek
                }
            }
        }
        return instanca;
    }
}
```

#### Dvostruka provera (Double-checked locking) — zašto?

```
Nit A: proveri instanca == null → true → ulazi u lock
Nit B: proveri instanca == null → true → čeka ispred locka
Nit A: pravi instancu → izlazi iz locka
Nit B: ulazi u lock → BEZ druge provere bi napravila DRUGU instancu!
Nit B: sa drugom proverom vidi instanca != null → ne pravi novu → OK ✓
```

#### Korišćenje u servisu

```csharp
public class MojService : MojServis.MojServisBase
{
    public override Task<Poruka> DodajStvar(Stvar request, ServerCallContext context)
    {
        Baza.Instanca().Podaci.Add(request.Id, request.Naziv);
        // Baza.Instanca() = uvek isti objekat, uvek isti Podaci rečnik
        return Task.FromResult(new Poruka { Text = "Dodato" });
    }
}
```

---

### Pattern 2 — Repository

#### Kada se koristi

Kada hoćeš da **odvojiš logiku pristupa podacima** od logike servisa. Servis ne zna kako se podaci čuvaju (u memoriji, u bazi, u fajlu) — to je briga Repository-ja. Olakšava testiranje i promenu načina čuvanja podataka.

#### Struktura

```csharp
// 1. Interfejs — definišeš koje operacije postoje
public interface IStudentiRepository
{
    bool Dodaj(int indeks, string imePrezime);
    bool Obrisi(int indeks);
    Student Preuzmi(int indeks);
    IEnumerable<Student> PreuzmiSve();
}

// 2. In-memory implementacija (koristi se u primerima sa predavanja)
public class InMemoryStudentiRepository : IStudentiRepository
{
    private readonly Dictionary<int, string> _baza = new();

    public bool Dodaj(int indeks, string imePrezime)
    {
        if (_baza.ContainsKey(indeks)) return false;
        _baza.Add(indeks, imePrezime);
        return true;
    }

    public bool Obrisi(int indeks)
    {
        return _baza.Remove(indeks);
    }

    public Student Preuzmi(int indeks)
    {
        if (!_baza.TryGetValue(indeks, out var ime)) return new Student();
        return new Student { BrojIndeksa = indeks, ImePrezime = ime };
    }

    public IEnumerable<Student> PreuzmiSve()
    {
        return _baza.Select(kv => new Student
        {
            BrojIndeksa = kv.Key,
            ImePrezime = kv.Value
        });
    }
}

// 3. Registracija u Program.cs (koristi Dependency Injection):
builder.Services.AddSingleton<IStudentiRepository, InMemoryStudentiRepository>();
// AddSingleton = jedna instanca repozitorija za ceo vek aplikacije

// 4. Korišćenje u servisu kroz constructor injection:
public class StudentiService : StudentServis.StudentServisBase
{
    private readonly IStudentiRepository _repo;

    public StudentiService(IStudentiRepository repo)
    // ASP.NET Core automatski "ubrizgava" repozitorij ovde
    {
        _repo = repo;
    }

    public override Task<Poruka> DodajStudenta(Student request, ServerCallContext context)
    {
        bool uspelo = _repo.Dodaj(request.BrojIndeksa, request.ImePrezime);
        return Task.FromResult(new Poruka
        {
            Text = uspelo ? "Dodat" : "Vec postoji"
        });
    }
}
```

#### Razlika između Singleton i Repository

| | Singleton | Repository |
|---|---|---|
| **Fokus** | Kako da postoji jedna instanca | Kako pristupiti podacima |
| **Problem koji rešava** | Deljenje podataka između poziva | Odvajanje logike od načina čuvanja |
| **Gde se vidi** | `Studenti.Instanca().Baza` | `_repo.Dodaj(...)` |
| **Testabilnost** | Teže testirati (globalno stanje) | Lako testirati (mokaš interfejs) |
| **U primerima** | DsGrpc_v1 (Elfak) | Naprednije implementacije |

#### Kombinovanje oba patterna

```csharp
// Repository koji koristi Singleton za čuvanje podataka:
public class InMemoryRepo : IStudentiRepository
{
    // Singleton osigurava jedinstvenost,
    // Repository osigurava čist interfejs za pristup
    private static readonly Dictionary<int, string> _baza = new();
    private static readonly object _lock = new();

    public bool Dodaj(int indeks, string ime)
    {
        lock (_lock)
        {
            if (_baza.ContainsKey(indeks)) return false;
            _baza.Add(indeks, ime);
            return true;
        }
    }
}
```

---

## Česte greške i rešenja

| Greška | Uzrok | Rešenje |
|--------|-------|---------|
| `Unimplemented` status kod | Metoda nije override-ovana u servisu | Dodaj `public override` za tu metodu |
| `Unavailable` na klijentu | Server nije pokrenut ili pogrešan port | Proveri da li server radi i koji port |
| Kompajler ne vidi generisane klase | Proto fajl nije dodat u `.csproj` | Dodaj `<Protobuf Include=...>` u csproj |
| Deadlock u bidirectional | Čitanje i pisanje se ne rade paralelno | Koristi `Task.Run()` za čitanje |
| Podaci se gube između poziva | Nema Singleton-a ili static podataka | Implementiraj Singleton ili DI |
| `CompleteAsync` nije pozvan | Zaboravljen kraj toka na klijentu | Uvek pozovi `RequestStream.CompleteAsync()` |

---

*Kraj priručnika — poslednja izmena na osnovu primera: DsGrpc_v1, grpc2026, Waver*
