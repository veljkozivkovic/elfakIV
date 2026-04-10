using Google.Protobuf.WellKnownTypes;
using Grpc.Core;

namespace Elfak.Services
{
    public class ElfakService : Elfak.ElfakBase
    // ElfakBase je automatski generisana klasa iz proto fajla.
    // Kada buildujemo projekat, protoc kompajler čita proto i pravi baznu klasu.
    // Mi je nasleđujemo i override-ujemo metode koje hoćemo da implementiramo.
    {
        // ========== DODAJ JEDNOG STUDENTA (Unary) ==========
        public override Task<Poruka> DodajStudenta(Student request, ServerCallContext context)
        // "override" = prepisujemo metodu iz bazne klase
        // Student request = poruka koju je klijent poslao (automatski deserijalizovana)
        // ServerCallContext context = informacije o pozivu (metapodaci, timeout...)
        // Task<Poruka> = asinhron zadatak koji vraća Poruka objekat
        {
            if (Studenti.Instanca().Baza.ContainsKey(request.BrojIndeksa))
            // Studenti.Instanca() = dohvatamo Singleton bazu
            // .Baza = rečnik studenata
            // .ContainsKey(request.BrojIndeksa) = proveravamo da li već postoji student
            // request.BrojIndeksa = vrednost polja brojIndeksa iz primljene Student poruke
            {
                return Task.FromResult(new Poruka { Text = "Student vec postoji" });
                // Task.FromResult() = pravi se završeni Task (ne čeka se ništa async)
                // new Poruka { Text = "..." } = object initializer sintaksa u C#
            }

            Studenti.Instanca().Baza.Add(request.BrojIndeksa, request.ImePrezime);
            // Dodajemo novog studenta u rečnik

            return Task.FromResult(new Poruka { Text = $"Student sa brojem indeksa {request.BrojIndeksa} uspesno dodat" });
            // $"..." = interpolovani string — umetamo vrednost varijable unutar stringa
        }

        // ========== DODAJ VIŠE STUDENATA (Bidirectional Streaming) ==========
        public override async Task DodajStudente(
            IAsyncStreamReader<Student> requestStream,    // čitač toka zahteva od klijenta
            IServerStreamWriter<Poruka> responseStream,  // pisač toka odgovora ka klijentu
            ServerCallContext context)
        // async Task = asinhrona metoda koja ne vraća vrednost direktno
        // IAsyncStreamReader = interfejs koji omogućava čitanje poruka jednu po jednu
        // IServerStreamWriter = interfejs koji omogućava pisanje poruka klijentu
        {
            await foreach (var student in requestStream.ReadAllAsync())
            // await foreach = asinhrono iteriramo kroz sve poruke koje klijent šalje
            // .ReadAllAsync() = čeka i čita sledeću poruku kada stigne
            // Petlja se završava kada klijent pozove CompleteAsync()
            {
                if (Studenti.Instanca().Baza.ContainsKey(student.BrojIndeksa)) continue;
                // "continue" = preskačemo ovog studenta ako već postoji (ne šaljemo grešku)

                Studenti.Instanca().Baza.Add(student.BrojIndeksa, student.ImePrezime);

                await responseStream.WriteAsync(new Poruka { Text = $"Student sa brojem indeksa {student.BrojIndeksa} uspesno dodat" });
                // await = čekamo da se poruka upiše u tok pre nego što nastavimo
                // Šaljemo potvrdu klijentu za SVAKI dodat student posebno
            }
        }

        // ========== OBRISI JEDNOG STUDENTA (Unary) ==========
        public override Task<Empty> ObrisiStudenta(Indeks request, ServerCallContext context)
        // Empty = Google-ov prazan tip (uvezli smo ga u proto fajlu)
        {
            if (!Studenti.Instanca().Baza.ContainsKey(request.BrojIndeksa))
            {
                return Task.FromResult(new Empty());
                // Ako student ne postoji, vraćamo prazno bez greške
            }

            Studenti.Instanca().Baza.Remove(request.BrojIndeksa);
            // Brišemo studenta iz rečnika

            return Task.FromResult(new Empty());
        }

        // ========== OBRISI VIŠE STUDENATA (Client Streaming) ==========
        public override async Task<Empty> ObrisiStudente(
            IAsyncStreamReader<Indeks> requestStream, // tok indeksa od klijenta
            ServerCallContext context)
        // Samo requestStream, nema responseStream — server šalje JEDAN odgovor na kraju
        {
            await foreach (var brojIndeksa in requestStream.ReadAllAsync())
            // Čitamo sve indekse koje klijent šalje
            {
                if (!Studenti.Instanca().Baza.ContainsKey(brojIndeksa.BrojIndeksa)) continue;
                Studenti.Instanca().Baza.Remove(brojIndeksa.BrojIndeksa);
            }

            return await Task.FromResult(new Empty());
            // Tek kad klijent završi sa slanjem, vraćamo Empty
        }

        // ========== PREUZMI JEDNOG STUDENTA (Unary) ==========
        public override Task<Student> PreuzmiStudenta(Indeks request, ServerCallContext context)
        {
            if (!Studenti.Instanca().Baza.ContainsKey(request.BrojIndeksa))
            {
                return Task.FromResult(new Student());
                // Vraćamo prazan Student objekat ako ne postoji
            }

            return Task.FromResult(new Student()
            {
                BrojIndeksa = request.BrojIndeksa,
                ImePrezime = Studenti.Instanca().Baza.GetValueOrDefault(request.BrojIndeksa)
                // .GetValueOrDefault() = vraća vrednost iz rečnika ili null ako ne postoji
            });
        }

        // ========== PREUZMI VIŠE STUDENATA (Server Streaming) ==========
        public override async Task PreuzmiStudente(
            IndeksOdDo request,                         // opseg indeksa
            IServerStreamWriter<Student> responseStream, // tok odgovora ka klijentu
            ServerCallContext context)
        {
            var studenti = Studenti.Instanca().Baza
                .OrderBy(s => s.Key)
                // .OrderBy() = sortiramo po ključu (broju indeksa) rastuće
                .Where(s => s.Key >= request.OdBrojaIndeksa && s.Key <= request.DoBrojaIndeksa);
            // .Where() = filtriramo samo studente čiji indeks je u traženom opsegu

            foreach (var sv in studenti)
            // sv = KeyValuePair<int, string> — par ključ/vrednost iz rečnika
            {
                await responseStream.WriteAsync(new Student
                {
                    BrojIndeksa = sv.Key,   // ključ rečnika = broj indeksa
                    ImePrezime = sv.Value   // vrednost rečnika = ime i prezime
                });
                // Šaljemo svakog studenta posebno u toku klijentu
            }
        }
    }
}
