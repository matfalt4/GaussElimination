#include<iostream>
#include<fstream>
using namespace std;

double** pobierzMacierz(string nazwa_pliku, int n, int m);
double* pobierzWyrazyWolne(string nazwa_pliku, int n);
void eliminacjaGaussa(double** A, double* b, int n, int m);
void podstawienieWsteczne(double** A, double* b, double* xs, int n);
int sprawdzUklad(double** A, double* b, int n);

int main()
{
    int n = 5; 

    double* Xs = new double[n];

    double** A = pobierzMacierz("/Users/mateuszfaltyn/Desktop/studia/MetodyNumeryczne/UkladyRownanLiniowych/macierz.txt", n, n);
    double* b = pobierzWyrazyWolne("/Users/mateuszfaltyn/Desktop/studia/MetodyNumeryczne/UkladyRownanLiniowych/wyrazyWolne.txt", n);

    cout << "Macierz poczatkowa:\n";
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << A[i][j] << " ";
        }
        cout << "| " << b[i] << endl;
    }
    cout << endl;

    eliminacjaGaussa(A, b, n, n);

    cout << "Po eliminacji Gaussa:\n";
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << A[i][j] << " ";
        }
        cout << "| " << b[i] << endl;
    }
    cout << endl;

    int status = sprawdzUklad(A, b, n);

    if(status == -1){
        cout << "Uklad sprzeczny (brak rozwiazan)\n";
    }
    else if(status == 0){
        cout << "Uklad nieoznaczony (nieskonczenie wiele rozwiazan)\n";
    }
    else{
        podstawienieWsteczne(A, b, Xs, n);

        cout << "Rozwiazanie:\n";
        for(int i = 0; i < n; i++){
            cout << "x[" << i << "] = " << Xs[i] << endl;
        }
    }

    for(int i = 0; i < n; i++){
        delete[] A[i];
    }
    delete[] A;
    delete[] b;
    delete[] Xs;

    return 0;
}

double** pobierzMacierz(string nazwa_pliku, int n, int m){
    ifstream plik(nazwa_pliku);
    double** macierz = new double*[n];
    for(int i = 0; i < n; i++) macierz[i] = new double[m];

    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            macierz[i][j] = 0;
        }
    }

    if(!plik){
        cout << "Nie udalo sie otworzyc pliku! " << endl;
        return nullptr;
    } else {
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                plik >> macierz[i][j];
            }
        }
        plik.close();
    }
    return macierz;
}

double* pobierzWyrazyWolne(string nazwa_pliku, int n){
    ifstream plik(nazwa_pliku);
    double* tab = new double[n];

    if(!plik){
        cout << "Nie udalo sie otworzyc pliku! " << endl;
        for(int i = 0; i < n; i++) tab[i] = 0;
        return tab;
    } else {
        for(int i = 0; i < n; i++) plik >> tab[i];
        plik.close();
    }
    return tab;
}

void eliminacjaGaussa(double** A, double* b, int n, int m){
    for(int k = 0; k < n-1; k++){
        int a = A[k][k];

        if(a == 0){
            for(int i = k + 1; i < n; i++){
                if(A[i][k] != 0){
                    for(int j = 0; j < n; j++){
                        std::swap(A[k][j], A[i][j]);
                    }
                    std::swap(b[k], b[i]);
                    break;
                }
            }
        }

        for(int i = k + 1; i < n; i++){
            double m = A[i][k]/a;

            for(int j = k; j < n; j++) A[i][j] = A[i][j] - m*A[k][j];

            b[i] = b[i] - m*b[k];
        }
    }
}

void podstawienieWsteczne(double** A, double* b, double* xs, int n){
    for(int i = n-1; i >= 0; i--){
        double suma = 0.;
        for(int j = i + 1; j < n; j++) suma += A[i][j]*xs[j]; 

        xs[i] = (b[i] - suma)/A[i][i];
    }
}

int sprawdzUklad(double** A, double* b, int n){
    bool nieskonczony = false;

    for(int i = 0; i < n; i++){
        bool sameZera = true;

        for(int j = 0; j < n; j++){
            if(A[i][j] != 0){
                sameZera = false;
                break;
            }
        }

        if(sameZera){
            if(b[i] != 0){
                return -1; // sprzeczny
            } else {
                nieskonczony = true;
            }
        }
    }

    if(nieskonczony) return 0;
    return 1;
}