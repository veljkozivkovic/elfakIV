// jun 2023 stara akreditacija pp ==========================================================================================
//3. zadatak: Napisati MPI program koji vrši paralelni upis i čitanje binarne datoteke, prema sledećim zahtevima:
//a) Prvi proces upisuje cele brojeve od 0 do K-1 od početka datoteke, drugi proces zapisuje brojeve od K sa pomerajem K u datoteci, itd. 
//   Koristiti pojedinačne pokazivače na datoteke. 
//b) Ponovo otvoriti datoteku. Svaki proces čita upravo upisane podatke koristeći funkcije sa eksplicitnim pomerajem. 
//   Proveriti da li je čitanje pravilno izvršeno.
//c) Svaki proces upisuje upravo pročitane podatke u novu datoteku, na način prikazan na slici, za K=6 i 3 aktivna procesa. 
//   Poslednje upisivanje u datoteku učiniti što efikasnijim.
#include "mpi.h"       
#include <stdio.h>
#include <stdlib.h>

#define K 6

int main (int argc, char* argv[])
{
    int size, rank;
    MPI_File fh;
    int buf[K];
    int rid[K];


    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for(int i = 0; i < K; i++)
    {
        buf[i] = K * rank + i;
    }

    //a
    MPI_File_open(MPI_COMM_WORLD, "jun23.dat", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);
    MPI_File_seek(fh, K * rank * sizeof(int), MPI_SEEK_SET);
    MPI_File_write(fh, buf, K, MPI_INT, MPI_STATUS_IGNORE);
    MPI_File_close(&fh);

    //b
    MPI_File_open(MPI_COMM_WORLD, "jun23.dat", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
    MPI_File_read_at(fh, rank * K * sizeof(int), read, K, MPI_INT, MPI_STATUS_IGNORE);
    MPI_File_close(&fh);

    for(int i = 0; i < K ; i++)
    {
        if(buf[i]!=rid[i])
        {   
            printf("ERROR");
            MPI_Finalize();
            return -1;
        }   
    }

    //c
    MPI_Type view;
    MPI_Type_vector(K / 2, 2, K, MPI_INT, &view);
    MPI_Type_commit(&view);

    MPI_File_open(MPI_COMM_WORLD, "jun23.dat", MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);
    MPI_File_set_view(fh, rank * 2 * sizeof(int), MPI_INT, view,"native", MPI_INFO_NULL );
    MPI_File_write_all(fh, read, K , MPI_INT, MPI_STATUS_IGNORE);
    MPI_File_close(&fh);

    MPI_Type_free(&view);
    MPI_Finalize();
    return 0;


}



// jun 2024 ==========================================================================================
//3. Napisati MPI program koji vrši paralelni upis i čitanje binarne datoteke, prema sledećim zahtevima:
//a) Svaki proces upisuje po 105 proizvoljnih celih brojeva u datoteku file1.dat. Upis se vrši upotrebom pojedinačnih pokazivača, dok redosled podataka u fajlu ide od podataka poslednjeg do podataka prvog procesa.
//b) Ponovo otvoriti datoteku. Svaki proces vrši čitanje upravo upisanih podataka upotrebom funkcija sa eksplicitnim pomerajem.
//c) Upravo pročitane podatke upisati u novu datoteku, na način prikazan na slici (za slučaj od 3 procesa).
//U poslednjem zahtevu posebno obratiti pažnju na efikasnost paralelizacije upisa.
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>


#define N 105


int main(int argc, char* argv[])
{
    int size, rank;
    MPI_File fh;
    int buf[N];
    int rid[N];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    for(int i = 0; i < N; i ++)
    {
        buf[i] = rank * N + i;
    }


    //a
    MPI_File_open(MPI_COMM_WORLD, "file1.dat", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);
    MPI_File_seek(fh, rank * N * sizeof(int), MPI_SEEK_SET);
    MPI_File_write(fh, buf, N, MPI_INT, MPI_STATUS_IGNORE);
    MPI_File_close(&fh);

    //b
    MPI_File_open(MPI_COMM_WORLD, "fil1.dat", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
    MPI_File_read_at(fh, rank * N * sizeof(int), rid, N, MPI_INT, MPI_STATUS_IGNORE);
    MPI_File_close(&fh);



    //c
    int n = 0:
    while (n * (n+1) / 2 < N)
    {
        n+=1;
    }
    
    int block_len[n];
    int block_disp[n];
    int offset = 0;
    for(int i = 0; i < n; i++)
    {
        offset += i * size; 

        block_len[i] = i + 1;
        block_disp[i] = offset + rank * (i+1) ;
        
    }

    MPI_Type view;

    MPI_Type_indexed(n, block_len, block_disp, MPI_INT, &view);
    MPI_Type_commit(&view);

    MPI_File_open(MPI_COMM_WORLD, "file1.dat", MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);
    MPI_File_set_view(fh, 0 , MPI_INT, view, "native", MPI_INFO_NULL);
    MPI_File_write_all(fh, read,n, MPI_INT, MPI_STATUS_IGNORE);
    MPI_File_close(&fh);


    MPI_Type_free(&view);
    MPI_Finalize();
    return 0;
}

// kol 1 2024 ==========================================================================================
//3. Napisati MPI program koji vrši paralelni upis i čitanje binarne datoteke, prema sledećim zahtevima:
//1. Upis se vrši upotrebom pojedinačnih pokazivača, dok redosled podataka u fajlu ide od podataka poslednjeg 
//do podataka prvog procesa.
//2. Ponovo otvoriti datoteku. Svi procesi vrše čitanje N 
//podataka iz datoteke dat.dat, tako da se ne može predvideti 
//koji će proces pročitati koji deo datoteke. Obratiti pažnju na konzistentnost prilikom čitanja.
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define N 6

int main(int argc, char* argv[])
{

    int size, rank;
    MPI_File fh;

    int buf[N];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for(int i = 0; i < N; i++)
    {
        buf[i] = i * rank * N;
    }


    //1
    MPI_File_open(MPI_COMM_WORLD, "dat.dat", MPI_MODE_CREATE | MPI_MODE_WRONLY, &fh);
    MPI_File_seek(fh, (size - rank - 1) * sizeof(int), MPI_SEEK_SET);
    MPI_File_write(fh, buf, N, MPI_INT, MPI_STATUS_IGNORE);
    MPI_File_close(&fh);

    //2

    MPI_File_open(MPI_COMM_WORLD, "dat.dat", MPI_MODE_RDONLY, &fh);
    MPI_File_read_shared(fh, buf, N, MPI_INT, MPI_STATUS_IGNORE);
    MPI_File_close(&fh);

    MPI_Finalize();
    return 0;
}

// okt 2 2025 ==========================================================================================
//Datoteka input.dat sadrži ukupno 10MB podataka. Napisati MPI program koji vrši obradu ovih podataka i 
//priprema ih za obradu, ujedno vršeći paralelni upis i čitanje datoteke. Na početku, svi procesi vrše čitanje 
//iste količine podataka tako da poslednji proces čita prvi skup podataka, pretposlednji proces drugi skup, itd. 
//korišćenjem funkcija sa pojedinačnim pokazivačem. Pročitane podatke procesi upisuju u datoteku output.dat, 
//tako što ih dele na četiri jednaka dela i upisuju po šemi prikazanoj na slici (za slučaj od 3 procesa), 
//pri čemu je potrebno obratiti pažnju na paralelizaciju tog upisa. Napomena: Zadatak rešiti korišćenjem osmobitnog tipa podataka.
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define TOTAL_SIZE (10 * 1024 * 1024)

int main(int argc, char* argv[])
{
    int size, rank;
    

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    MPI_File fh;

    int chunkSize = TOTAL_SIZE / size;
    int partSize = chunkSize / 4;

    unsigned char* buf = malloc(chunkSize);


    MPI_File_open(MPI_COMM_WORLD, "input.dat", MPI_RDONLY,MPI_INFO_NULL ,&fh);
    MPI_File_seek(fh, (size - rank - 1) * chunkSize, MPI_SEEK_SET);
    MPI_File_read(fh, buf, chunkSize , MPI_BYTE, MPI_STATUS_IGNORE);
    MPI_File_close(&fh);


    MPI_Datatype view;
    MPI_Type_vector(4, partSize, partSize * size, MPI_BYTE, &view);
    MPI_Type_commit(&view);

    MPI_File_open(MPI_COMM_WORLD, "out.dat", MPI_MODE_WRONLY | MPI_MODE_CREATE,MPI_INFO_NULL, &fh);
    MPI_File_set_view(fh, rank * partSize, MPI_BYTE, view, "native", MPI_INFO_NULL);
    MPI_File_write_all(fh, buf, chunkSize, MPI_BYTE, MPI_STATUS_IGNORE);
    MPI_File_close(&fh);

    MPI_Type_free(&view);
    MPI_Finalize();
    free(buf);
    return 0;

}




