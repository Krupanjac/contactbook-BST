//
// Created by da170706d on 19-Oct-24.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;


struct Contact {
    string prezime;
    string ime;
    unsigned long long telefon;
    Contact* next;

};


class Loader {
public:
    Loader(string fileName) {
        head = nullptr;
        brojKontakta = 0;
        ucitajIzFajla(fileName);
        obrisiDuplikat();
    }

    unsigned long long stringToUnsignedLongLong(const string& telefon_str) {
        unsigned long long result = 0;
        for (char c : telefon_str) {
            if (!isdigit(c)) {
                throw invalid_argument("Greska - Karakter koji nije digit!");
            }

            result = result * 10 + (c - '0');
        }
        return result;
    }

    void ucitajIzFajla(const string& f) {
        ifstream file(f);
        string line;

        if (!file.is_open()) {
            cout << "Greska u ucitavanju fajla!" << endl;
            return;
        }
        //skip f-line
        getline(file, line);

        //iteriraj
        while (getline(file, line)) {
            string ime, prezime, telefon_str;
            unsigned long long telefon_num = 0;
            stringstream s(line);

            getline(s, prezime, ',');
            getline(s, ime, ',');
            getline(s, telefon_str, ',');

            //telefon_str.erase(telefon_str.find_last_not_of(" \n\r\t") + 1);
            //telefon_str.erase(0, telefon_str.find_first_not_of(" \n\r\t"));

           // cout<<"this is telefon_str:"<<telefon_str<<" This is length: "<<telefon_str.length();
            telefon_num = stringToUnsignedLongLong(telefon_str);
            //cout<<"This is telefon_num:"<<telefon_num<<endl;
            //cout<<endl;

            dodajKontakt(prezime, ime, telefon_num);

        }
        file.close();
    }

    void obrisiDuplikat() {
        Contact* trenutni = getHead();

        while (trenutni) {
            Contact* trenutniCompare = getHead();
            Contact* prev = nullptr;

            while (trenutniCompare) {
                if ((trenutni->telefon == trenutniCompare->telefon) && (trenutni->ime != trenutniCompare->ime) && (trenutni->prezime != trenutniCompare->prezime)) {
                    cout << "Kontakt: " << trenutni->prezime << " " << trenutni->ime << " " << trenutni->telefon << endl;
                    cout << "Kontakt sa istim brojem: " << trenutniCompare->prezime << " " << trenutniCompare->ime << " " << trenutniCompare->telefon << endl;
                    cout << "Brisem: " << trenutniCompare->prezime << " " << trenutniCompare->ime << endl;
                    prev->next = trenutniCompare->next;
                    delete trenutniCompare;
                    trenutniCompare = prev->next;
                    setBrojacSub();

                }
                prev = trenutniCompare;
                trenutniCompare = trenutniCompare->next;
            }
            trenutni = trenutni->next;

        }
    }



    void dodajKontakt(string& prezime, string& ime, unsigned long long telefon) {
        Contact* novi = new Contact{ prezime,ime,telefon,nullptr };

        if (!getHead()) setHead(novi);
        else {
            Contact* trenutni = getHead();
            while (trenutni->next) {
                trenutni = trenutni->next;
            }
            trenutni->next = novi;
        }
        setBrojacAdd();
    }

    void stampajKontakt() {
        Contact* temp = getHead();
        while (temp) {
            cout << "Prezime: " << temp->prezime << " ,Ime: " << temp->ime << " ,Telefon: " << temp->telefon << endl;
            temp = temp->next;
        }
        cout << "Broj Kontakta: " << getBrojac();
    }


    Contact* getHead() {
        return this->head;
    }

    void setHead(Contact* newHead) {
        head = newHead;
    }

    unsigned getBrojac() {
        return this->brojKontakta;
    }

    void setBrojacAdd() {
        brojKontakta++;
    }

    void setBrojacSub() {
        brojKontakta--;
    }

private:
    Contact* head;
    unsigned brojKontakta;
};



template <typename T>
class Stek {
private:
    T* podaci;
    int velicina;
    int top;

public:
    Stek(int size) {
        velicina = size;
        podaci = new T[velicina];
        top = 0;
    }

    ~Stek() {
        delete[] podaci;
    }


    void push(T val) {
        if (top == velicina) {
            cout << "Stek popunjen";
            return;
        }
        podaci[top++] = val;
    }

    T pop() {
        if (top > 0) return podaci[--top];
        else cout << "Stek je prazan";
    }

    int empty() {
        if (top == 0) return 1;
        return 0;
    }

};


struct Cvor {
    string prezime;
    string ime;
    unsigned long long telefon;
    Cvor* levi;
    Cvor* desni;
    Cvor* otac;

    Contact* poruka;
    unsigned long long *sadrzaj = nullptr;
    unsigned long long *brojacSadrzaj = nullptr;
    int size = 0;
};


class BinarnoStablo {
public:

    BinarnoStablo() {
        root = nullptr;
        //stek = nullptr;
    }

    void setRoot(Cvor* novi) {
        this->root = novi;
    }

    Cvor* getRoot() {
        return this->root;
    }

    Cvor*& getRootRef() {
        return this->root;
    }

    Contact* dohvatiIzListe(Contact* gl, int mid) {
        Contact* head = gl;
        for (int i = 0; i < mid; i++) {
            head = head->next;
        }
        return head;
    }



    void sortedListToBST(Loader* list) {
        int low = 0, high = list->getBrojac();
        Stek<pair<int,int>> stek(high);


        stek.push(make_pair(low, high - 1));

        while (!stek.empty()) {
            pair<int, int> range = stek.pop();
            int l = range.first;
            int h = range.second;


            if (l > h) continue; // Invalid range, skip


            int mid = (l + h) / 2;


            Contact* trenutni = dohvatiIzListe(list->getHead(), mid);


            Cvor* noviCvor = new Cvor{ trenutni->prezime, trenutni->ime, trenutni->telefon, nullptr, nullptr, nullptr , nullptr};


            if (getRoot() == nullptr) {
                setRoot(noviCvor);
            }
            else {

                Cvor* current = getRoot();
                Cvor* parent = nullptr;

                while (current) {
                    parent = current;
                    if (noviCvor->prezime < current->prezime) {
                        current = current->levi;
                    }
                    else {
                        current = current->desni;
                    }
                }

                noviCvor->otac = parent;

                if (noviCvor->prezime < parent->prezime) {
                    parent->levi = noviCvor; // levo dete
                }
                else {
                    parent->desni = noviCvor; // desno dete
                }
            }


            stek.push(make_pair(mid + 1, h)); // desni opseg
            stek.push(make_pair(l, mid - 1));  // levi opseg
        }
    }
    static void brisanje(Cvor* cvorZaBrisanje, Cvor*& root) {
        Cvor* trenutni = cvorZaBrisanje;



        Cvor* rp = nullptr;
        if (!trenutni) {
            cout << "Error - Korisnik nije pronadjen." << endl;
            return;
        }

        //cout << "Ovo je korisnik: " << trenutni->prezime << " " << trenutni->ime << endl;
        //Samo desno dete
        if (!trenutni->levi) rp = trenutni->desni;
        //Samo levo dete
        else if (!trenutni->desni) rp = trenutni->levi;
        // Oba potomka
        else {
            Cvor* f = trenutni;
            rp = trenutni->desni;
            Cvor* s = rp->levi;
            while (s) {
                f = rp;
                rp = s;
                s = rp->levi;
            }
            if (f != trenutni) {
                f->levi = rp->desni;
                rp->desni = trenutni->desni;
                if (rp->desni) rp->desni->otac = rp;
            }
            rp->levi = trenutni->levi;
            if (rp->levi) rp->levi->otac = rp;
        }

        //preulancavanje
        if (!trenutni->otac) root = rp;
        else if (trenutni == trenutni->otac->levi) trenutni->otac->levi = rp;
        else trenutni->otac->desni = rp;

        cout << "Brisem korisnika: " << trenutni->prezime << " " << trenutni->ime << endl;

        delete trenutni;
        //cout << "Brisanje uspesno zavrseno." << endl;
    }



    Cvor* pretraga(string prezime, string ime = "") {
        Cvor* trenutni = this->getRoot();
        if (ime == "") {
            while ((trenutni != nullptr) && (trenutni->ime != ime) && (trenutni->prezime != prezime)) {
                if (trenutni->prezime > prezime)
                    trenutni = trenutni->levi;
                else
                    trenutni = trenutni->desni;

            }
        }
        else {
            while ((trenutni != nullptr) && (trenutni->prezime != prezime)) {
                if (trenutni->prezime > prezime)
                    trenutni = trenutni->levi;
                else
                    trenutni = trenutni->desni;

            }

        }
        return trenutni;

    }

    void stampajPoruke(Cvor *glava) {
        Contact *trenutni = glava->poruka;
        cout<<"====================="<<endl;
        while(trenutni) {
            cout<<trenutni->prezime<<" "<<trenutni->ime<<", "<<trenutni->telefon<<endl;
            trenutni = trenutni->next;
        }
        cout<<"====================="<<endl;
    }



    void proveraOpsega(Contact* &poruka, int n) {
        Contact* trenutni = poruka;
        Contact* pret = nullptr;
        int i = 0;
        while(trenutni && i<n) {
            pret = trenutni;
            trenutni = trenutni->next;
            i++;
        }

        if(pret) pret->next = nullptr;

        while(trenutni) {
            Contact *tmp = trenutni;
            trenutni=trenutni->next;
            delete tmp;
        }
    }

    static void stampajSadrzaj(Cvor *gl){
    cout<<"===================="<<endl;
    for(int i = 0; i<gl->size;i++) cout<<gl->sadrzaj[i]<<" ("<<gl->brojacSadrzaj[i]<<")"<<endl;
    cout<<"===================="<<endl;

    }


    static void transpozicija(Cvor* &gl, int n, int i){
        Cvor* trenutni = gl;
        if(i!=0 && trenutni->brojacSadrzaj[i]<n){
        unsigned long long tmp = trenutni->sadrzaj[i];
        unsigned long long tmp2 = trenutni->brojacSadrzaj[i];

        trenutni->sadrzaj[i] = trenutni->sadrzaj[i-1];
        trenutni->brojacSadrzaj[i] = trenutni->brojacSadrzaj[i-1];


        trenutni->sadrzaj[i-1] = tmp;
        trenutni->brojacSadrzaj[i-1] = tmp2;
        }

        else if(i!=0 && trenutni->brojacSadrzaj[i]>=n){
            unsigned long long tmp = trenutni->sadrzaj[i];
            unsigned long long tmp2 = trenutni->brojacSadrzaj[i];

            for(int j = i; j>0; j--) {
                trenutni->sadrzaj[j] = trenutni->sadrzaj[j-1];
                trenutni->brojacSadrzaj[j] = trenutni->brojacSadrzaj[j-1];
            }
            trenutni->sadrzaj[0] = tmp;
            trenutni->brojacSadrzaj[0] = tmp2;
        }
    }



    static void novSadrzaj(Cvor* &gl, int n, unsigned long long fon) {
        Cvor* trenutni = gl;

        //istorija deljenja prazna
        if(!trenutni->sadrzaj) {
            int i = 1;
            trenutni->sadrzaj = new unsigned long long[i];
            trenutni->brojacSadrzaj = new unsigned long long[i];

            trenutni->sadrzaj[i-1] = fon;
            trenutni->brojacSadrzaj[i-1] = i;
            trenutni->size++;
            return;
        }

        else {
            for(int i =0; i<trenutni->size;i++) {
                if(trenutni->sadrzaj[i]==fon) {
                    trenutni->brojacSadrzaj[i]++;
                    transpozicija(trenutni, n, i);
                    return;
                }
            }

            unsigned long long* novi_sadrzaj = new unsigned long long[trenutni->size + 1];
            unsigned long long* novi_brojacSadrzaj = new unsigned long long[trenutni->size + 1];


        for (int i = 0; i < trenutni->size; i++) {
            novi_sadrzaj[i] = trenutni->sadrzaj[i];
            novi_brojacSadrzaj[i] = trenutni->brojacSadrzaj[i];
        }

        novi_sadrzaj[trenutni->size] = fon;
        novi_brojacSadrzaj[trenutni->size] = 1;

        delete trenutni->sadrzaj;
        delete trenutni->brojacSadrzaj;

        trenutni->sadrzaj = novi_sadrzaj;
        trenutni->brojacSadrzaj = novi_brojacSadrzaj;

        trenutni->size++;

        }


    }



    void novaPoruka(string prezime, string ime, unsigned long long fon, int n, Cvor * &cvor) {
        Cvor *glava = cvor;

        Contact *pret = nullptr;
        Contact* novi = new Contact{ prezime,ime,fon,nullptr};


        if(!glava->poruka) glava->poruka = novi;
        else {
            Contact* trenutni = glava->poruka;
            //case 1, da li postoji komunikacija?

            while(trenutni!=nullptr) {

                if((trenutni->prezime == prezime)&&(trenutni->ime==ime)&&(trenutni->telefon==fon)) {
                    //postoji istorija kom

                    if(trenutni==glava->poruka) return; //Kontakt je na pocetku liste

                    if(pret!=nullptr) {
                        pret->next = trenutni->next;
                        trenutni->next = glava->poruka;
                        glava->poruka = trenutni;
                    }
                    //proveri da li je opseg legitiman
                    proveraOpsega(glava->poruka,n);
                    return;
                }

                pret = trenutni;
                trenutni = trenutni->next;

            }
            //dodaj novu istoriju zapisa
            novi->next = glava->poruka;
            glava->poruka = novi;

            //proveri da li je opseg legitiman
            proveraOpsega(glava->poruka,n);

        }

    }




    void umetanje(string prezime, string ime, unsigned long long fon) {
        Cvor* novi = new Cvor{ prezime, ime, fon, nullptr, nullptr, nullptr, nullptr };
        Cvor* trenutni = this->getRoot();
        Cvor* parent = nullptr;

        // Case 1: Da li isti broj postoji
        while (trenutni != nullptr) {
            if (!uniqueTelefon(fon)) {
                cout << "Error - Postoji kontakt sa ovim brojem telefona!" << endl;
                delete novi;
                return;
            }

            parent = trenutni;

            // Case 2: Ako su ime i prezime isti, prethodnik
            if (trenutni->prezime == prezime && trenutni->ime == ime) {

                novi->levi = trenutni->levi;
                trenutni->levi = novi;
                novi->otac = trenutni;
                return;
            }

            // Nastavi pretragu
            if (prezime < trenutni->prezime) {
                trenutni = trenutni->levi;
            }
            else if (prezime > trenutni->prezime) {
                trenutni = trenutni->desni;
            }
            else {
                if (ime < trenutni->ime) {
                    trenutni = trenutni->levi;
                }
                else {
                    trenutni = trenutni->desni;
                }
            }
        }

        // Case 3: Regularan insert
        novi->otac = parent;

        if (parent == nullptr) {
            setRoot(novi);
        }
        else if (prezime < parent->prezime) {
            parent->levi = novi;
        }
        else if (prezime > parent->prezime) {
            parent->desni = novi;
        }
        else {
            if (ime < parent->ime) {
                parent->levi = novi;
            }
            else {
                parent->desni = novi;
            }
        }
    }

    bool uniqueTelefon(unsigned long long fon) {

        Cvor* trenutni, * prethodni;
        trenutni = this->getRoot();

        if (trenutni) {
            trenutni = trenutni;

            while (trenutni) {

                if (!trenutni->levi) {
                    if (trenutni->telefon == fon) return 0;
                    trenutni = trenutni->desni;
                }
                else {

                    prethodni = trenutni->levi;
                    while ((prethodni->desni != nullptr) && (prethodni->desni != trenutni)) prethodni = prethodni->desni;

                    if (!prethodni->desni) {
                        prethodni->desni = trenutni;
                        trenutni = trenutni->levi;
                    }

                    else {
                        prethodni->desni = nullptr;
                        if (trenutni->telefon == fon) return 0;
                        trenutni = trenutni->desni;
                    }


                }


            }

        }
        else {
            cout<<"==================="<<endl;
            cout << "Error - Case 6, NULLPTR" << endl;
            cout<<"==================="<<endl;
            return 0;
        }
        return 1;
    }

    void preOrderDeleteS(Cvor*& root,int n) {
        if (!root) return;

        Stek<Cvor*> stack(n);
        Stek<Cvor*> deleteStack(n);

        stack.push(root);

        while (!stack.empty()) {
            Cvor* trenutni = stack.pop();


            if (trenutni) {
                deleteStack.push(trenutni);



                if (trenutni->desni)
                    stack.push(trenutni->desni);

                if (trenutni->levi)
                    stack.push(trenutni->levi);


            }
        }

        while(!deleteStack.empty()) {
            Cvor *zaBrisanje = deleteStack.pop();
            brisanje(zaBrisanje,root);
        }
        root = nullptr;
    }

    void preOrderMT() {

        Cvor* trenutni, * prethodni;
        trenutni = this->getRoot();

        if (trenutni) {
            while (trenutni) {
                if (!trenutni->levi) {
                    cout << trenutni->prezime << " " << trenutni->ime << " " << trenutni->telefon << endl;
                    trenutni = trenutni->desni;
                }
                else {

                    prethodni = trenutni->levi;
                    while ((prethodni->desni != nullptr) && (prethodni->desni != trenutni)) prethodni = prethodni->desni;

                    if (!prethodni->desni) {
                        prethodni->desni = trenutni;
                        cout << trenutni->prezime << " " << trenutni->ime << " " << trenutni->telefon << endl;
                        trenutni = trenutni->levi;
                    }

                    else {
                        prethodni->desni = nullptr;
                        trenutni = trenutni->desni;
                    }
                }
            }
        }
        else {
            cout<<"==================="<<endl;
            cout << "Error - Case 6, NULLPTR" << endl;
            cout<<"==================="<<endl;
            return;
        }
    }


    void inOrderMT() {

        Cvor* trenutni, * prethodni;
        trenutni = this->getRoot();

        if (trenutni) {
            while (trenutni) {
                if (!trenutni->levi) {
                    cout << trenutni->prezime << " " << trenutni->ime << " " << trenutni->telefon << endl;
                    trenutni = trenutni->desni;
                }
                else {

                    prethodni = trenutni->levi;
                    while ((prethodni->desni != nullptr) && (prethodni->desni != trenutni)) prethodni = prethodni->desni;

                    if (!prethodni->desni) {
                        prethodni->desni = trenutni;
                        trenutni = trenutni->levi;
                    }

                    else {
                        prethodni->desni = nullptr;
                        cout << trenutni->prezime << " " << trenutni->ime << " " << trenutni->telefon << endl;
                        trenutni = trenutni->desni;
                    }
                }
            }
        }
        else {
            cout<<"==================="<<endl;
            cout << "Error - Case 6, NULLPTR" << endl;
            cout<<"==================="<<endl;
            return;
        }
    }

private:

    Cvor* root;


};


int main() {
    int sw = 0, n = 0, x = 0;
    string fileName;
    Loader* lista = nullptr;
    BinarnoStablo* bst = nullptr;

    while(true){
    cout<<"1. Inicjalizacija kontakta"<<endl<<"2. Posalji poruku kontaktu"<<endl<<"3. Posalji sadrzaj kontaktu"<<endl<<"4. Izlaz"<<endl<<"Izbor: ";
    cin>>sw;
    switch(sw){
    case 1: {
       if(!bst){
            int sw2 = 0;
            cout << "1. Contacts20.txt " << endl;
            cout << "2. Contacts100.txt " << endl;
            cout << "3. noviFajl.txt " << endl;
            cout << "Izbor: "; cin >> sw2;
            if (sw2 == 1) {
                fileName = "Contacts20.txt";
                lista = new Loader(fileName);
                bst = new BinarnoStablo();
                bst->sortedListToBST(lista);
            }
            else if (sw2 == 2) {
                fileName = "Contacts100.txt";
                lista = new Loader(fileName);
                bst = new BinarnoStablo();
                bst->sortedListToBST(lista);
            }
            else if (sw2 == 3) {
                string fileName3;
                cout << "Ime datoteke(bez .txt): "; cin >> fileName3;
                fileName3 = fileName3 + ".txt";

                lista = new Loader(fileName3);
                bst = new BinarnoStablo();
                bst->sortedListToBST(lista);
            }
            else {
                cout<<"==================="<<endl;
                cout << "Error - Pogresan unos!!!" << endl;
                cout<<"==================="<<endl;
            }

            cout<<endl<<"Ogranicenje istorije(n): ";cin>>n;
            cout<<endl<<"Ogranicenje deljenja(x): ";cin>>x;
       }
        else {
            cout<<"Error - inicializacija je vec izvrsena!"<<endl;
        }
            break;
    }
        case 2: {
        string prezime, ime;
        unsigned long long fon;

        cout<<"Prezime - Ime kontakta kojem se salje poruka"<<endl;
        cout<<"Prezime: ";cin>>prezime;
        cout<<"Ime: ";cin>>ime;
        Cvor *cvor = bst->pretraga(prezime,ime);

        if(cvor) {
            cout<<"Prezime - Ime - Telefon kontakta koji salje poruku"<<endl;
            cout<<"Prezime: ";cin>>prezime;
            cout<<"Ime: ";cin>>ime;
            cout<<"Telefon: ";cin>>fon;
            bst->novaPoruka(prezime,ime,fon,n,cvor);
            bst->stampajPoruke(cvor);
        }
        else {
            cout<<"==================="<<endl;
            cout<<"Error - kontakt nije pronadjen!"<<endl;
            cout<<"==================="<<endl;
        }

        break;

        }

        case 3: {

        string prezime, ime;
        unsigned long long fon;
        //Balle Danna
        cout<<"Prezime - Ime kontakta kojem se salje sadrzaj"<<endl;
        cout<<"Prezime: ";cin>>prezime;
        cout<<"Ime: ";cin>>ime;
        Cvor *cvor = bst->pretraga(prezime,ime);

        if(cvor) {
            cout<<"Telefon kontakta koji salje sadrzaj"<<endl;
            cout<<"Telefon: ";cin>>fon;
            BinarnoStablo::novSadrzaj(cvor,x,fon);
            BinarnoStablo::stampajSadrzaj(cvor);
        }
        else {
            cout<<"==================="<<endl;
            cout<<"Error - kontakt nije pronadjen!"<<endl;
            cout<<"==================="<<endl;
        }

        break;
        }

        case 4: {
        bst->preOrderDeleteS(bst->getRootRef(),lista->getBrojac());
        delete bst;
        delete lista;
        exit(0);

        break;
        }


        case 1337: {
        //Kirlin Christos 2237082105

        bst->novaPoruka("Balle","Danna",5012064229, n, bst->getRootRef());
        bst->novaPoruka("Bohlin","Kriste",6771919493, n, bst->getRootRef());
        bst->novaPoruka("Bonin","Leonard",5231253452, n, bst->getRootRef());
        bst->novaPoruka("Flaunier","Norean",8931855542, n, bst->getRootRef());
        bst->novaPoruka("Gallichiccio","Dolph",2109248980, n, bst->getRootRef());
        bst->novaPoruka("Gianni","Herbert",2109248980, n, bst->getRootRef());
        bst->novaPoruka("Flaunier","Norean",8931855542, n, bst->getRootRef());
        bst->novaPoruka("Flaunier","Norean",8931855542, n, bst->getRootRef());
        bst->stampajPoruke(bst->getRoot());

        break;
        }


        case 420: {
        Cvor *tren = bst->pretraga("Balle","Danna");
        BinarnoStablo::novSadrzaj(tren,x,5012064229);
        BinarnoStablo::novSadrzaj(tren,x,6771919493);
        BinarnoStablo::novSadrzaj(tren,x,5231253452);
        BinarnoStablo::novSadrzaj(tren,x,2109248980);
        BinarnoStablo::novSadrzaj(tren,x,8931855542);

        BinarnoStablo::stampajSadrzaj(tren);

        BinarnoStablo::novSadrzaj(tren,x,5231253452);
        BinarnoStablo::novSadrzaj(tren,x,1337);
        BinarnoStablo::novSadrzaj(tren,x,1337);
        BinarnoStablo::novSadrzaj(tren,x,1234);
        BinarnoStablo::novSadrzaj(tren,x,5678);
        BinarnoStablo::novSadrzaj(tren,x,5231253452);
        BinarnoStablo::novSadrzaj(tren,x,5231253452);
        BinarnoStablo::novSadrzaj(tren,x,2109248980);
        BinarnoStablo::novSadrzaj(tren,x,8931855542);
        BinarnoStablo::novSadrzaj(tren,x,5231253452);
        BinarnoStablo::novSadrzaj(tren,x,1337);


        BinarnoStablo::stampajSadrzaj(tren);

        break;
        }

        default: {
        cout<<"==================="<<endl;
        cout<<"Error - pogresan unos!"<<endl;
        cout<<"==================="<<endl;
        break;
        }
      }
    }




    return 1;
}