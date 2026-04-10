using Elfak.Services;
// Uvozimo namespace gde su naši servisi definisani

var builder = WebApplication.CreateBuilder(args);
// Kreira se "builder" za ASP.NET Core web aplikaciju
// args = argumenti komandne linije

builder.Services.AddGrpc();
// Registruje gRPC u DI (Dependency Injection) kontejner aplikacije
// Bez ovoga, gRPC ne bi radio

var app = builder.Build();
// Gradi se konačna aplikacija sa svim registrovanim servisima

app.MapGrpcService<GreeterService>();
// Registruje GreeterService kao gRPC endpoint
// Sada klijenti mogu da pozivaju SayHello metodu

app.MapGrpcService<ElfakService>();
// Registruje ElfakService sa svim studentskim operacijama
// Redosled registracije nije važan

app.MapGet("/", () => "Communication with gRPC...");
// Ako neko otvori browser i ode na "/", dobija ovu poruku
// gRPC ne može da se koristi iz browsera direktno

app.Run();
// Pokreće server — blokira i čeka zahteve beskonačno