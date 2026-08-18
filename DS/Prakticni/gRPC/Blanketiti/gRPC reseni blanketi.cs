# gRPC NAPOMENE
# kaze steva paterne trazi, njima se pao repository pattern


# .proto je uvek isti za klijenta i servera, smao je razlika u:
# option csharp_namespace = "Elfak"; // za klijent "ElfakClient"
# samo treba proto i service servera



===================2024 jun=====================

//protobuf.proto:

syntax = "proto3";
import "google/protobuf/empty.proto";
package elfak;
option csharp_namespace = "Elfak";


service Elfak{

    rpc SendMessage(Poruka) returns (google.protobuf.Empty);
    rpc DeleteMessage(Identifikator) returns (Poruka);
    rpc ListMessages(google.protobuf.Empty) returns (stream Poruka);

}


message Poruka 
{
    string text = 1;
}

message Identifikator
{
    int32 id = 1;
}


// pomocna klasa

public class Poruke
{

    public ConcurrentDictionary<int, string> Baza {get;set;}

    private static Poruke instanca;

    private static objcet lockObj = new object();
    
    private int counter = 0;

    private Poruke()
    {
        Baza = new ConcurrentDictionary<int,string>();
        Baza.TryAdd(counter++, "Cao Mare");
        Baza.TryAdd(counter++, "Cao Vrki");

    }

    public static Poruke Instanca()
    {
        
        if(instanca == null)
        {
            lock(lockObj)
            {
                instanca = new Poruke();
            }

        }

        return instanca;
    }

    public bool Dodaj(string poruka)
    {
        return Baza.TryAdd(counter++, poruka)

    }


}


 rpc SendMessage(Poruka) returns (google.protobuf.Empty);
    rpc DeleteMessage(Identifikator) return (Poruka);
    rpc ListMessages(google.protobuf.empty) returns (stream Poruka);

// Service
namespace Elfak;
public class ElfakService : Elfak.ElfakBase
{

    public override Task<Empty> SendMessage (Poruka request,
                                             ServerCallContext context)
    {
        
        Poruke.Instanca().Dodaj(request.Text);


    }

    public override Task<Poruka> DeleteMessage(Identifikator request,
                                               ServerCallContext context)
    {
        bool izbrisano = Poruke.Instanca().Baza.TryRemove(request.Id);
        if(izbrisano)
            Task.FromResult(new Poruka { Text = "Uspesno izbrisana poruka sa id: " + request.Id});
        else
            Task.FromResult(new Poruka { Text = "Ne postoji poruka sa datim idem"});
    }

    public override async Task ListMessages( Empty request,
                                        IServerStreamWriter<Poruka> streamWriter,
                                        ServerCallContext context)
    {
        
        foreach(var p in Poruke.Instanca().Baza)
        {
            
            await streamWriter.WriteAsync(new Poruka ( Text = p.Key + " : " + p.Value);

        }


    }

}



===================2025 okt2====================

//protobuf.proto:

syntax = "proto3";
import "google/protobuf/empty.proto";
option csharp_namespace = "Elfak";
package elfak;

service Elfak{
    rpc IspitajPar(Par) returns (Poruka);
    rpc IspitajParove( stream Par) returns (stream Poruka);
}

message Par{
    int32 a = 1;
    int32 b = 2;
}

message Poruka{
    string text = 1; // mora malo pocetno slovo
}


//Service:

namespace Elfak;
public class ElfakService : Elfak.ElfakBase
{
    public ovverride Task<Poruka> IspitajPar(
                                              Par request,
                                              ServerCallContext context)
    {
        
        if(request.A * request.A == request.B)
            return Task.FromResult(new Poruka {Text = "Da"});
        else
            return Task.FromResult(new Poruka{Text = "Ne"});


    }


    public override async IspitajParove(IAsyncStreamReader<Par> requestStream,
                                        IServerStreamWriter<Poruka> responseStream,
                                        ServerCallContext context)
    {
        
        for(var p in requestStream.ReadAllAsync())
        {
            if(p.A * p.A == p.B) // TREBA VELIKO A I B
                responseStream.WriteAsync(new Poruka{Text ="Da"});
            else
                responseStream.WriteAsync(new Poruka{Text ="Ne"});
        }

    }

}





===================2024 kol1===================

//protobuf.proto
syntax = "proto3";
import "google/protobuf/empty.proto"
option csharp_namespace = "Elfak";
package elfak;


service Elfak{
    rpc AddTask(Poruka) returns (Poruka);
    rpc ListTasks(google.protobuf.Empty) returns (stream Zadatak);
    rpc MarkTaskAsCompleted(Id) returns (Poruka);
}


message Poruka
{
    string text = 1;
}

message Id
{
    int32 vrednost = 1;
}

message Zadatak
{
    int32 id = 1;
    string naziv = 2;
    bool gotov = 3;
}

// pomocna klasa
public class Zadaci
{
    
    private static Zadaci instanca;
    private static object lockObj = new object();
    private static int counter = 0;
    public ConcurrentDictionary<int, (string naziv, bool zavrseno)> Baza; 

    private Zadaci()
    {
        Baza = new ConcurrentDictionary<int, (string naziv, bool zavrseno)>();
        Baza.TryAdd(counter++, ("Pranje Zuba", false);
        Baza.TryAdd(counter++, ("Pranje Kose", true);
    }

    public static Instanca()
    {
        if(instanca == null)
        {
            lock(lockObj)
            {
                instanca = new Zadaci();
            }
        }
        return instanca;
    }

    public bool Dodaj(string task)
    {
        lock(lockObj)
        {
            bool uspesno = Baza.TryAdd(counter++, (task, false));
        }
        return uspesno;
    }

}

//service

namespace Elfak;
public class ElfakService:Elfak.ElfakBase
{
    public override Task<Poruka> AddTask( Poruka request,
                                          ServerCallContext context)
    {
        
        bool uspesno = Zadaci.Instanca().Dodaj(request.Text);
        if(uspesno)
        {
            return Task.FromResult(new Poruka(Text = {"Uspesno dodat task"});
        }
        else
        {
            return Task.FromResult(new Poruka(Text = {"Neuspesno dodat task"});

        }
    }

    public override async Task ListTasks(Empty request,
                                         IServerStreamWriter<Zadatak> responseStream,
                                         ServerCallContext context)
    {
        
        foreach(var t in Zadaci.Instanca().Baza)
        {
            await responseStream.WriteAsync(new Zadatak {
                                                            Id = t.Key,
                                                            Naziv = t.Value.naziv,
                                                            zavrsen = t.Value.zavrsen
                                                        });

        }


    }
    public override Task<Poruka> MarkTaskAsCompleted(Id request,
                                                     ServerCallContext context)
    {
        
        if(Zadaci.Instanca().Baza.TryGetValue(request.Vrednost, out var stara) )
        {
            Zadaci.Instanca().Baza[request.Vrednost] = (stara.naziv, true);
            return Task.FromResult(new Poruka { Text = "Uspesno je completovan task"} );
        }
        else
        {
            return Task.FromResult(new Poruka { Text = "Neuspesno je completovan task, mozda je pogresan id" } );
        }

    }



}


