//
// Created by da170706d on 15-Oct-24.
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


            Cvor* noviCvor = new Cvor{ trenutni->prezime, trenutni->ime, trenutni->telefon, nullptr, nullptr, nullptr };


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

    void umetanje(string prezime, string ime, unsigned long long fon) {
        Cvor* novi = new Cvor{ prezime, ime, fon, nullptr, nullptr, nullptr };
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
            cout << "Error - Case 6, NULLPTR" << endl;
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
            cout << "Error - Case 6, NULLPTR" << endl;
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
            cout << "Error - Case 6, NULLPTR" << endl;
            return;
        }
    }

private:
    //Contact* stek;

    Cvor* root;


};


int main() {
    int sw = 0;
    string fileName;
    Loader* lista = nullptr;
    BinarnoStablo* bst = nullptr;

    while (true) {
        sw = 0;
        cout << endl;
        cout << "1. Ucitavanje imenika i formiranje BST " << endl;
        cout << "2. Pronadji kontakt po prezimenu (ili prezimenu i imenu) " << endl;
        cout << "3. Izmena kontakta " << endl;
        cout << "4. Dodavanje novog kontakta " << endl;
        cout << "5. Uklanjanje kontakta (prezime i ime) " << endl;
        cout << "6. Formatiran ispis sadrzaja " << endl;
        cout << "7. Obrisi stablo iz memorije " << endl;
        cout << "8. Izlaz " << endl;
        cout << "Izbor: ";
        cin >> sw;
        cout << endl;

        switch (sw) {
        case 1: {
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
                //delete lista;
                //lista = nullptr;
            }
            else if (sw2 == 2) {
                fileName = "Contacts100.txt";
                lista = new Loader(fileName);
                bst = new BinarnoStablo();
                bst->sortedListToBST(lista);
                //delete lista;
                //lista = nullptr;
            }
            else if (sw2 == 3) {
                string fileName3;
                cout << "Ime datoteke(bez .txt): "; cin >> fileName3;
                fileName3 = fileName3 + ".txt";

                lista = new Loader(fileName3);
                bst = new BinarnoStablo();
                bst->sortedListToBST(lista);
                //delete lista;
                //lista = nullptr;
            }
            else {
                cout << "Pogresan unos!!!" << endl;
            }
            break;
        }

        case 2: {
            string pr, im;

            cout << "Prezime: "; cin >> pr;
            cout << "Enter ime (pritisni Enter da preskocis): ";
            cin.ignore();  // preskoci ako prazan
            getline(cin, im);

            if (im.empty()) {
                Cvor* temp = bst->pretraga(pr);  // Call prezime
                if (temp)
                    cout << "Pronadjen kontakt - " << "Prezime: " << temp->prezime << " " << "Ime: " << temp->ime <<" Telefon: "<<temp->telefon<<endl;
                else cout << "Korisnik nije pronadjen!";

            }
            else {
                Cvor* temp = bst->pretraga(pr, im);  // Call + ime
                if (temp)
                    cout << "Pronadjen kontakt - " << "Prezime: " << temp->prezime << " " << "Ime: " << temp->ime <<" Telefon: "<<temp->telefon<<endl;
                else cout << "Korisnik nije pronadjen!";
            }
            break;
        }
        case 3: {
            string pr, im;

            cout << "Prezime: "; cin >> pr;
            cout << endl;
            cout << "Ime: "; cin >> im;
            cout << endl;
            if (bst) {
                Cvor* temp = bst->pretraga(pr, im);  // Call + ime
                if (temp) {
                    int izb = 0;
                    cout << "Pronadjen kontakt - " << "Prezime: " << temp->prezime << " " << "Ime: " << temp->ime << " Telefon: " << temp->telefon << endl;
                    cout << "1. Prezime" << endl << "2. Ime" << endl << "3. Prezime i Ime" << endl << "4. Telefon" << endl << "Izbor: ";
                    cin >> izb;
                    if (izb == 1) {
                        string str;

                        cout << "Unesi novo prezime za korisnika " << "'" << temp->prezime << "'" << " '" << temp->ime << "'" << " '" << temp->telefon << "': "; cin >> str;

                        string prezimeOld = temp->prezime,imeOld = temp->ime;
                        unsigned long long telefonOld = temp->telefon;

                        BinarnoStablo::brisanje(temp,bst->getRootRef());
                        bst->umetanje(str,imeOld,telefonOld);

                        //temp->prezime = str;

                    }
                    else if (izb == 2) {
                        string str;
                        cout << "Unesi novo ime za korisnika " << "'" << temp->prezime << "'" << " '" << temp->ime << "'" << " '" << temp->telefon << "': "; cin >> str;
                        string prezimeOld = temp->prezime,imeOld = temp->ime;
                        unsigned long long telefonOld = temp->telefon;

                        BinarnoStablo::brisanje(temp,bst->getRootRef());
                        bst->umetanje(prezimeOld,str,telefonOld);
                    }
                    else if (izb == 3) {
                        string str, str2;
                        cout << "Unesi novo prezime i ime za korisnika " << "'" << temp->prezime << "'" << " '" << temp->ime << "'" << " '" << temp->telefon << "'" << endl;
                        cout << "Prezime: "; cin >> str;
                        cout << endl;
                        cout << "Ime: "; cin >> str2;
                        cout << endl;
                        string prezimeOld = temp->prezime,imeOld = temp->ime;
                        unsigned long long telefonOld = temp->telefon;

                        BinarnoStablo::brisanje(temp,bst->getRootRef());
                        bst->umetanje(str,str2,telefonOld);
                    }
                    else if (izb == 4) {
                        unsigned long long fon = 0;
                        cout << "Unesi novi broj telefona za korisnika " << "'" << temp->prezime << "'" << " '" << temp->ime << "'" << " '" << temp->telefon << "': "; cin >> fon;
                        if (bst->uniqueTelefon(fon)) temp->telefon = fon;
                        else cout << "Error - Telefon vec postoji!" << endl;

                    }
                    else {
                        cout << "Pogresan izbor!!!" << endl;
                    }

                }

                else cout << "Error - Stablo ne postoji." << endl;

                break;
            }
            else {
                cout << "Error - Stablo ne postoji";
            }
        }

        case 4: {
            if (bst) {
                string prez, im;
                unsigned long long fon;

                cout << "Unesi novi kontakt" << endl;
                cout << "Prezime: "; cin >> prez;
                cout << endl;
                cout << "Ime:"; cin >> im;
                cout << endl;
                cout << "Telefon:"; cin >> fon;
                cout << endl;
                bst->umetanje(prez, im, fon);
            }
            else cout << "Error - Stablo ne postoji." << endl;


            break;
        }

        case 5: {
            string pr, im;
            cout << "Unesi prezime i ime kontakta za brisanje" << endl << "Prezime: "; cin >> pr;
            cout << "Ime: "; cin >> im;
            BinarnoStablo::brisanje(bst->pretraga(pr, im), bst->getRootRef());
            break;
        }

        case 6: {
            int sw;
            cout<<"1. inOrder"<<endl<<"2. preOrder"<<endl<<"Izbor: ";cin>>sw;
            if(sw==1) {
                if (bst) bst->inOrderMT();
                else cout << "Error - Stablo ne postoji." << endl;
                cout<<"================"<<endl;
            }
            else if(sw==2) {
                if (bst) bst->preOrderMT();
                else cout << "Error - Stablo ne postoji." << endl;
                cout<<"================"<<endl;
            }

            else cout<<"Error - Pogresan unos!"<<endl;

            break;
        }

        case 7: {
            if (bst) {
                bst->preOrderDeleteS(bst->getRootRef(),lista->getBrojac());
                bst = nullptr;
                delete lista;
                lista = nullptr;
            }

            else cout << "Error - Stablo ne postoji." << endl;

            break;
        }
        case 8: {
            delete bst;
            delete lista;
            lista = nullptr;
            bst = nullptr;
            exit(0);
            break;
        }

        default: {
            cout << "=======================" << endl;
            cout << "Pogresan unos!" << endl;
            cout << "=======================" << endl;
            break;
        }


        }
    }

    return 1;
}