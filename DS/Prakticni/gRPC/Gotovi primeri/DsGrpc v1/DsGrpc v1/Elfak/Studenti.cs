public class Studenti
{
    public Dictionary<int, string> Baza { get; set; }
    // Dictionary = mapa ključ→vrednost. Ključ je int (broj indeksa),
    // vrednost je string (ime i prezime studenta)

    private static Studenti instanca;
    // "static" znači da je ovo JEDNA varijabla za celu klasu, ne po objektu
    // Ovo je deo Singleton šablona — čuva jedinu instancu klase

    private static object lockObj = new object();
    // Objekat koji služi za zaključavanje (lock) u višenitnom okruženju
    // Sprečava da dve niti istovremeno prave novu instancu

    private Studenti()
    // "private" konstruktor — znači da NIKO SPOLJA ne može da napravi
    // new Studenti(). Jedino Singleton metoda može.
    {
        Baza = new Dictionary<int, string>()
        {
                { 100, "Milos Milosevic" },
                { 105, "Mihajlo Mihajlovic" },
                { 107, "Djurdja Djuric" },
                { 110, "Marko Markovic" },
                { 120, "Milutin Misic" },
                { 121, "Milica Micic" }
            // ... inicijalni podaci
        };
        // Ovo je inicijalizacija rečnika sa podacima koji postoje od starta
    }

    public static Studenti Instanca()
    // Ovo je Singleton metoda. Jedini način da dođeš do objekta Studenti.
    // "static" = poziva se na klasi, ne na objektu: Studenti.Instanca()
    {
        if (instanca == null)
        // Prva provera: ako instanca ne postoji, ulazimo u blok
        {
            lock (lockObj)
            // Zaključavamo — samo jedna nit može ući ovde u isto vreme
            // Bez ovoga, dve niti bi ISTOVREMENO videle instanca==null
            // i napravile dve instance — što narušava Singleton
            {
                if (instanca == null)
                // Druga provera posle zaključavanja (Double-checked locking)
                // Jer kad su dve niti čekale, prva pravi instancu,
                // a druga kad uđe treba da PONOVO proveri — možda je već napravljena
                {
                    instanca = new Studenti();
                }
            }
        }
        return instanca;
        // Uvek vraćamo istu jedinu instancu
    }
}