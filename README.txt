Voicu Catalin Marian
Grupa 322CC

Am lucrat pe scheletul din laboratorul 9.
Am adaugat fisierele parson.c si parson.h din enuntul temei.

Codul este o implementare a unui client ce trimite request-uri catre un server
HTTP si primeste raspunsuri de la acesta. Clientul este implementat in fisierul
client.c, iar functiile necesare pentru a trimite request-uri si a primi raspunsuri
sunt implementate in fisierele request.c si request.h. Functiile necesare pentru
a parsa raspunsurile primite de la server sunt implementate in fisierul parson.c,
iar declaratiile acestora sunt in fisierul parson.h.

Ce am facut in request.h:
Am adaugat scheletul functiei:
    char *compute_delete_request(char *host, char *url, char *query_params, char *token);

Ce am facut in request.c:
Am terminat de facut functia compute_get_request:
    Aceasta functie este folosita pentru a calcula si a returna un sir de caractere ce
    reprezinta un request de tip GET. Parametrii host si url specifica adresa si calea 
    catre server, specificate in enuntul temei. Parametrul query_params specifica
    parametrii de query, daca este cazul, acestia pot fi NULL. Parametrii cookies si 
    token sunt folositi pentru a specifica cookie-urile si token-ul de autorizare, daca
    este cazul, acestia pot fi NULL. Functia returneaza un sir de caractere ce reprezinta 
    request-ul de tip GET. Sirul de caractere returnat trebuie sa fie alocat dinamic.

Am terminat de facut functia compute_post_request:
    Aceasta functie este folosita pentru a calcula si a returna un sir de caractere ce
    reprezinta un request de tip POST. Parametrii host si url specifica adresa si calea 
    catre server, specificate in enuntul temei. Parametrul content_type specifica tipul
    continutului trimis in request, iar parametrul body_data specifica continutul 
    propriu-zis trimis in request. Parametrii cookies si token sunt folositi pentru a
    specifica cookie-urile si token-ul de autorizare, daca este cazul, acestia pot 
    fi NULL. Functia returneaza un sir de caractere ce reprezinta request-ul de tip POST.
    Sirul de caractere returnat trebuie sa fie alocat dinamic.


Am terminat de facut functia compute_delete_request:
    Aceasta functie este folosita pentru a calcula si a returna un sir de caractere ce
    reprezinta un request de tip DELETE. Parametrii host si url specifica adresa si calea
    catre server, specificate in enuntul temei. Parametrul query_params specifica
    parametrii de query, daca este cazul, acestia pot fi NULL. Parametul token este folosit
    pentru a specifica token-ul de autorizare, daca este cazul, acesta poate fi NULL.
    Functia returneaza un sir de caractere ce reprezinta request-ul de tip DELETE.
    Sirul de caractere returnat trebuie sa fie alocat dinamic.

Ce am facut in client.c:
    Am initializat variabilele host, url, query_params, cookies, token, content_type si
    body_data, cu specificatiile din enuntul temei. Am initializat si conditiile de iesire 
    din program, conditia care verifica daca un client este logat sau nu, precum si conditia 
    care verifica   daca un client are acces la biblioteca sau nu.

    Cu ajutorul unei bucle while, care verifica daca clientul trimite comanda exit, am 
    implementat comenzile de register, login, enter_library, get_books, get_book, add_book,
    delete_book, logout si exit.

    Pentru fiecare comanda, inainte de toate se afiseaza mesajul afisat in tema pentru
    comanda respectiva.
    
    Pentru comenzile register, login, get_book, add_book si delete_book verificam daca 
    inputul de la tastatura este null, daca este nul intoarce un mesaj si clientul trebuie
    sa introduca alt input.

    Pentru comenziile get_book, pentru partea in care se introduce numarul de pagini de la 
    comanda  add_book si delete_book, verificam si daca inputul este un numar, daca nu este 
    un numar intoarce un mesaj si clientul trebuie sa introduca alt input.

    Pentru fiecare comanda se formeaza un mesaj care sa fie trimis la client, iar pentru 
    comenziile: register, login si add_book se analizeaza inputul de la tastatura cu 
    ajutorul functiilor din JSON.

    Pentru comanda register, daca inputul este valid, se trimite un request de tip POST
    catre server, iar daca raspunsul primit de la server este 201 Created, se afiseaza
    un mesaj de succes, iar daca raspunsul primit de la server este 409 Conflict, se
    afiseaza un mesaj de eroare.

    Pentru comanda login, se verifica daca clientul este logat sau nu, iar daca este logat
    se afiseaza un mesaj de eroare, iar daca nu este logat, se verifica daca inputul este
    valid, iar daca este valid se trimite un request de tip POST catre server, iar daca
    raspunsul primit de la server este 200 OK, se afiseaza un mesaj de succes, iar daca
    raspunsul primit de la server este 401 Unauthorized, se afiseaza un mesaj de eroare.
    Variabila logged_in este setata pe 1 si variabila cookie este setata cu cookie-ul
    primit de la server.

    Pentru comanda enter_library, se verifica daca clientul este logat sau nu, iar daca
    nu este logat se afiseaza un mesaj de eroare, iar daca este logat, se trimite un
    request de tip GET catre server, iar daca raspunsul primit de la server este 200 OK,
    se afiseaza un mesaj de succes, iar daca raspunsul primit de la server este 401
    Unauthorized, se afiseaza un mesaj de eroare. Variabila has_library este setata pe 1
    si variabila token este setata cu token-ul primit de la server.

    Pentru comanda get_books, se verifica daca clientul este logat sau nu, iar daca nu
    este logat se afiseaza un mesaj de eroare, iar daca este logat, se verifica daca
    clientul are acces la biblioteca sau nu, iar daca nu are acces se afiseaza un mesaj
    de eroare, iar daca are acces, se trimite un request de tip GET catre server, iar
    daca raspunsul primit de la server este 200 OK, se afiseaza un mesaj de succes, iar
    daca raspunsul primit de la server este 401 Unauthorized, se afiseaza un mesaj de
    eroare.

    Pentru comanda get_book, se verifica daca clientul este logat sau nu, iar daca nu
    este logat se afiseaza un mesaj de eroare, iar daca este logat, se verifica daca
    clientul are acces la biblioteca sau nu, iar daca nu are acces se afiseaza un mesaj
    de eroare, iar daca are acces, se verifica daca inputul este valid, iar daca este
    valid, se trimite un request de tip GET catre server, iar daca raspunsul primit de
    la server este 200 OK, se afiseaza un mesaj de succes, iar daca raspunsul primit de
    la server este 401 Unauthorized, se afiseaza un mesaj de eroare.

    Pentru comanda add_book, se verifica daca clientul este logat sau nu, iar daca nu
    este logat se afiseaza un mesaj de eroare, iar daca este logat, se verifica daca
    clientul are acces la biblioteca sau nu, iar daca nu are acces se afiseaza un mesaj
    de eroare, iar daca are acces, se verifica daca inputul este valid, iar daca este
    valid, se trimite un request de tip POST catre server, iar daca raspunsul primit de
    la server este 201 Created, se afiseaza un mesaj de succes, iar daca raspunsul primit
    de la server este 401 Unauthorized, se afiseaza un mesaj de eroare.

    Pentru comanda delete_book, se verifica daca clientul este logat sau nu, iar daca nu
    este logat se afiseaza un mesaj de eroare, iar daca este logat, se verifica daca
    clientul are acces la biblioteca sau nu, iar daca nu are acces se afiseaza un mesaj
    de eroare, iar daca are acces, se verifica daca inputul este valid, iar daca este
    valid, se trimite un request de tip DELETE catre server, iar daca raspunsul primit de
    la server este 200 OK, se afiseaza un mesaj de succes, iar daca raspunsul primit de
    la server este 401 Unauthorized, se afiseaza un mesaj de eroare.

    Pentru comanda logout, se verifica daca clientul este logat sau nu, iar daca nu este
    logat se afiseaza un mesaj de eroare, iar daca este logat, se trimite un request de
    tip GET catre server, iar daca raspunsul primit de la server este 200 OK, se afiseaza
    un mesaj de succes, iar daca raspunsul primit de la server este 401 Unauthorized, se
    afiseaza un mesaj de eroare. Variabilele logged_in si library_acces sunt setate pe 0
    si variabile cookie si token sunt setate pe null.

    Pentru comanda exit, schimbam valoarea lui exit cu 1, logged_in cu 0 si library_acces
    cu 0, iar daca exit este 1, se iese din while si se inchide clientul.

    In cazul in care comanda introdusa nu este valida se afiseaza un mesaj cu toate
    comenziile valide.