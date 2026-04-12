//el mejor codigo de todos :D, si estuvo dificil//
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// se usan struct para asiganrle nombre a las variables con diferente tipo de variable //
struct Equipo {
    string nombre;
    int PJ, PG, PE, PP, GF, GC, DG, PTS;
};

struct Partido {
    int jornada, goles_local, goles_visitante;
    string fecha, equipo_local, equipo_visitante;
};

struct ConfigLiga {
    string nombre;
    int pts_victoria, pts_empate, pts_derrota;
    string equipos[30]; 
    int num_equipos = 0;
};

// estas son las varables globales del programa //

ConfigLiga config;
Partido partidos[400]; 
int total_partidos = 0;
Equipo tabla[100];
int total_equipos_tabla = 0;


// esta funcion busca el final de las cadenas para saber cuano acaban //
int largo_cadena(string s) {
    int l = 0;
    while (s[l] != '\0') l++;
    return l;
}

// quita espacios y basura de los extremos cualquier texto de forma manual, tambien evita que hayan diferencias entre los largos y los espacios de los textos de Windows a Linux //
string limpia(string texto) {
    int inicio = 0, fin = largo_cadena(texto) - 1;
    while (inicio <= fin && (texto[inicio] == ' ' || texto[inicio] == '\r')) inicio++;
    while (fin >= inicio && (texto[fin] == ' ' || texto[fin] == '\r' || texto[fin] == '\n')) fin--;
    
    string resultado = "";
    for (int i = inicio; i <= fin; i++) resultado += texto[i];
    return resultado;
}

// esta funcion cuenta cuantas letras ocupa un numero para que la tabla no se mueva ni tenga disvarianzas y quede bien alineada //
int contar_digitos(int n) {
    if (n == 0) return 1;
    int cont = 0;
    if (n < 0) { cont++; n = -n; }
    while (n > 0) { cont++; n = n / 10; }
    return cont;
}

// esta funcion lee y carga los datos desde los archivos en conjunto hasta la linea 176 //

bool lee_config(string ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) return false;
    
    config.num_equipos = 0;
    string palabra;

    while (archivo >> palabra) {
        palabra = limpia(palabra);
        if (palabra == "" || palabra[0] == '#') continue;

        int pos_igual = -1;
        int tam = largo_cadena(palabra);
        for (int i = 0; i < tam; i++) {
            if (palabra[i] == '=') { pos_igual = i; break; }
        }

        if (pos_igual != -1) {
            string clave = "";
            for (int i = 0; i < pos_igual; i++) clave += palabra[i];
            string valor = "";
            for (int i = pos_igual + 1; i < tam; i++) valor += palabra[i];
            
            clave = limpia(clave);
            valor = limpia(valor);

            if (clave == "nombre") config.nombre = valor;
            else if (clave == "puntos_victoria" || clave == "puntos_empate" || clave == "puntos_derrota") {
                int num = 0;
                int tam_v = largo_cadena(valor);
                for (int i = 0; i < tam_v; i++) {
                    if (valor[i] >= '0' && valor[i] <= '9') num = num * 10 + (valor[i] - '0');
                }
                if (clave == "puntos_victoria") config.pts_victoria = num;
                else if (clave == "puntos_empate") config.pts_empate = num;
                else if (clave == "puntos_derrota") config.pts_derrota = num;
            }
            else if (clave == "equipo") {
                if (config.num_equipos < 30) {
                    config.equipos[config.num_equipos] = valor;
                    config.num_equipos++;
                }
            }
        }
    }
    archivo.close();
    return config.num_equipos > 0;
}

void lee_partidos(string ruta) {
    ifstream archivo(ruta);
    total_partidos = 0;
    if (!archivo.is_open()) return;

    string bloque;
    while (archivo >> bloque) {
        string campos[6] = {"", "", "", "", "", ""};
        int campo_actual = 0;
        int tam_bloque = largo_cadena(bloque);

        for (int i = 0; i < tam_bloque; i++) {
            if (bloque[i] == '|') { if (campo_actual < 5) campo_actual++; }
            else { campos[campo_actual] += bloque[i]; }
        }

        if (campos[0] != "" && total_partidos < 400) {
            Partido p;
            for (int c = 0; c < 6; c++) {
                if (c == 0 || c == 4 || c == 5) {
                    int num = 0;
                    string s = limpia(campos[c]);
                    int tam_s = largo_cadena(s);
                    for (int i = 0; i < tam_s; i++) {
                        if (s[i] >= '0' && s[i] <= '9') num = num * 10 + (s[i] - '0');
                    }
                    if (c == 0) p.jornada = num;
                    else if (c == 4) p.goles_local = num;
                    else if (c == 5) p.goles_visitante = num;
                }
            }
            p.fecha = limpia(campos[1]);
            p.equipo_local = limpia(campos[2]);
            p.equipo_visitante = limpia(campos[3]);
            partidos[total_partidos] = p;
            total_partidos++;
        }
    }
    archivo.close();
}

void muestra_jornadas(string ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) return;
    string palabra;
    while (archivo >> palabra) {
        bool es_etiqueta = false;
        int tam = largo_cadena(palabra);
        if (tam == 11 && palabra[0]=='F'&&palabra[1]=='I'&&palabra[2]=='N') es_etiqueta = true;
        if (tam >= 8 && palabra[0]=='J'&&palabra[1]=='O'&&palabra[2]=='R') es_etiqueta = true;
        if (tam >= 6 && palabra[0]=='F'&&palabra[1]=='E'&&palabra[2]=='C') es_etiqueta = true;

        string p_limpia = limpia(palabra);
        if (p_limpia != "") {
            cout << p_limpia;
            if (es_etiqueta) cout << "\n";
            else cout << " ";
        }
    }
    archivo.close();
    cout << "\n";
}

// aqui se usan punteros *equipo para asignarle a cada equipo alguna accion en la tabla y se creo la tabla //
void actualiza_equipo(Equipo *equipo, int gf, int gc) {
    (*equipo).PJ++;
    (*equipo).GF += gf;
    (*equipo).GC += gc;
    if (gf > gc) { (*equipo).PG++; (*equipo).PTS += config.pts_victoria; }
    else if (gf == gc) { (*equipo).PE++; (*equipo).PTS += config.pts_empate; }
    else { (*equipo).PP++; (*equipo).PTS += config.pts_derrota; }
    (*equipo).DG = (*equipo).GF - (*equipo).GC;
}

void construye_tabla() {
    total_equipos_tabla = config.num_equipos;
    for (int i = 0; i < total_equipos_tabla; i++) {
        tabla[i].nombre = config.equipos[i];
        tabla[i].PJ = tabla[i].PG = tabla[i].PE = tabla[i].PP = 0;
        tabla[i].GF = tabla[i].GC = tabla[i].DG = tabla[i].PTS = 0;
    }
    for (int i = 0; i < total_partidos; i++) {
        for (int j = 0; j < total_equipos_tabla; j++) {
            if (tabla[j].nombre == partidos[i].equipo_local)
                actualiza_equipo(&tabla[j], partidos[i].goles_local, partidos[i].goles_visitante);
            if (tabla[j].nombre == partidos[i].equipo_visitante)
                actualiza_equipo(&tabla[j], partidos[i].goles_visitante, partidos[i].goles_local);
        }
    }
}

// en esta funcion creo el desempate por el gol diferencia y los goles a favor como factores de desempate//
void ordena_tabla() {
    for (int i = 0; i < total_equipos_tabla - 1; i++) {
        for (int j = i + 1; j < total_equipos_tabla; j++) {
            bool intercambiar = false;
            if (tabla[i].PTS < tabla[j].PTS) intercambiar = true;
            else if (tabla[i].PTS == tabla[j].PTS) {
                if (tabla[i].DG < tabla[j].DG) intercambiar = true;
                else if (tabla[i].DG == tabla[j].DG && tabla[i].GF < tabla[j].GF) intercambiar = true;
            }
            if (intercambiar) {
                Equipo aux = tabla[i];
                tabla[i] = tabla[j];
                tabla[j] = aux;
            }
        }
    }
}

// este parte de la funcion alinea por "celdas" o lineas exactas la tabla para que se vea ordenada //

void imprimir_fila_tabla(int pos, string n, int v[], int dg, int pts, ostream &salida) {
    salida << "  ";
    if (pos < 10) salida << " "; salida << pos << " | ";
    
    salida << n;
    int espacios = 24 - largo_cadena(n);
    for (int s = 0; s < espacios; s++) salida << " ";
    
    for (int k = 0; k < 6; k++) {
        salida << " | ";
        if (contar_digitos(v[k]) < 2) salida << " ";
        salida << v[k];
    }
    
    salida << " | ";
    int char_dg = contar_digitos(dg);
    if (dg >= 0) char_dg++; 
    for (int s = 0; s < 3 - char_dg; s++) salida << " ";
    if (dg >= 0) salida << "+";
    salida << dg << " | ";
    
    if (contar_digitos(pts) < 2) salida << " ";
    salida << pts << "\n";
}

void muestra_tabla() {
    cout << "\n  # | Equipo                   | PJ | PG | PE | PP | GF | GC | DG | PTS\n";
    cout << "----|--------------------------|----|----|----|----|----|----|----|----\n";
    for (int i = 0; i < total_equipos_tabla; i++) {
        int v[6] = {tabla[i].PJ, tabla[i].PG, tabla[i].PE, tabla[i].PP, tabla[i].GF, tabla[i].GC};
        imprimir_fila_tabla(i + 1, tabla[i].nombre, v, tabla[i].DG, tabla[i].PTS, cout);
    }
}

// se toma una imagen de la tabla para guardarlo en tabla.txt //
void exportar_tabla() {
    ofstream archivo("data/tabla.txt");
    if (!archivo.is_open()) { cout << "Error creando tabla.txt\n"; return; }
    
    archivo << "  # | Equipo                   | PJ | PG | PE | PP | GF | GC | DG | PTS\n";
    archivo << "----|--------------------------|----|----|----|----|----|----|----|----\n";
    for (int i = 0; i < total_equipos_tabla; i++) {
        int v[6] = {tabla[i].PJ, tabla[i].PG, tabla[i].PE, tabla[i].PP, tabla[i].GF, tabla[i].GC};
        imprimir_fila_tabla(i + 1, tabla[i].nombre, v, tabla[i].DG, tabla[i].PTS, archivo);
    }
    archivo.close();
    cout << "\nTabla exportada exitosamente en data/tabla.txt\n";
}
// almacena todo el historial de partidos y se los puede registrar por numero de jornada y fecha, una de las funciones opcionales //
void historial_enfrentamientos() {
    
    for(int i=0; i<config.num_equipos; i++) cout << i+1 << ". " << config.equipos[i] << "\n";
    int id1, id2;
    cout << "ID Equipo 1: "; cin >> id1;
    cout << "ID Equipo 2: "; cin >> id2;
    
    if (id1 < 1 || id1 > config.num_equipos || id2 < 1 || id2 > config.num_equipos) return;
    string eq1 = config.equipos[id1-1];
    string eq2 = config.equipos[id2-1];
    
    cout << "\n--- Partidos entre " << eq1 << " y " << eq2 << " ---\n";
    bool hay = false;
    for (int i = 0; i < total_partidos; i++) {
        if ((partidos[i].equipo_local == eq1 && partidos[i].equipo_visitante == eq2) ||
            (partidos[i].equipo_local == eq2 && partidos[i].equipo_visitante == eq1)) {
            cout << "J" << partidos[i].jornada << " | " << partidos[i].fecha << " | " 
                 << partidos[i].equipo_local << " " << partidos[i].goles_local << "-" 
                 << partidos[i].goles_visitante << " " << partidos[i].equipo_visitante << "\n";
            hay = true;
        }
    }
    if (!hay) cout << "Nunca han jugado.\n";
}

void registrar_partido() {
    int jornada; string fecha; int id_local, id_vis, gl, gv;
    cout << "Numero de Jornada: "; cin >> jornada;
    cout << "Fecha (DD/MM): "; cin >> fecha;
    
    for(int i=0; i<config.num_equipos; i++) cout << i+1 << ". " << config.equipos[i] << "\n";
    cout << "ID Local: "; cin >> id_local;
    cout << "ID Visitante: "; cin >> id_vis;
    
    if (id_local == id_vis || id_local < 1 || id_local > config.num_equipos || id_vis < 1 || id_vis > config.num_equipos) {
        cout << "Equipos invalidos.\n"; return;
    }
    
    string n_local = config.equipos[id_local-1];
    string n_vis = config.equipos[id_vis-1];

    // aqui se revisa que no se repitan partidos //
    for (int i = 0; i < total_partidos; i++) {
        if (partidos[i].jornada == jornada) {
            if ((partidos[i].equipo_local == n_local && partidos[i].equipo_visitante == n_vis) ||
                (partidos[i].equipo_local == n_vis && partidos[i].equipo_visitante == n_local)) {
                cout << "\n[ERROR] Ese partido ya se jugo en la jornada " << jornada << "!\n";
                return;
            }
        }
    }

    cout << "Goles Local: "; cin >> gl;
    cout << "Goles Visitante: "; cin >> gv;

    // este guardado manual en modo Append //
    ofstream arch_p("data/partidos.txt", ios_base::app);
    arch_p << jornada << "|" << fecha << "|" << n_local << "|" << n_vis << "|" << gl << "|" << gv << "\n";
    arch_p.close();

    ofstream arch_f("data/fechas.txt", ios_base::app);
    arch_f << "JORNADA=" << jornada << "\nFECHA=" << fecha << "\n";
    arch_f << n_local << " " << gl << " - " << gv << " " << n_vis << "\nFIN_JORNADA\n";
    arch_f.close();

    cout << "Partido registrado.\n";
}

// esta funcion es para volver al archivo y dar la opcion de editar un resultado ya escrito si se desea //
void editar_resultado() {
    int j, id_l, id_v;
    cout << "Jornada del partido a editar: "; cin >> j;
    for(int i=0; i<config.num_equipos; i++) cout << i+1 << ". " << config.equipos[i] << "\n";
    cout << "ID Equipo Local: "; cin >> id_l;
    cout << "ID Equipo Visitante: "; cin >> id_v;
    
    if (id_l < 1 || id_v < 1) return;
    string eq1 = config.equipos[id_l-1];
    string eq2 = config.equipos[id_v-1];
    
    int index = -1;
    for (int i = 0; i < total_partidos; i++) {
        if (partidos[i].jornada == j && partidos[i].equipo_local == eq1 && partidos[i].equipo_visitante == eq2) {
            index = i; break;
        }
    }
    
    if (index != -1) {
        cout << "Nuevos goles de " << eq1 << ": "; cin >> partidos[index].goles_local;
        cout << "Nuevos goles de " << eq2 << ": "; cin >> partidos[index].goles_visitante;
        
        ofstream arch("data/partidos.txt");
        for(int i=0; i<total_partidos; i++){
            arch << partidos[i].jornada << "|" << partidos[i].fecha << "|" 
                 << partidos[i].equipo_local << "|" << partidos[i].equipo_visitante << "|"
                 << partidos[i].goles_local << "|" << partidos[i].goles_visitante << "\n";
        }
        arch.close();
        cout << "Partido editado correctamente.\n";
    } else {
        cout << "Partido no encontrado.\n";
    }
}

// "interfaz" de el menu y sus opciones ///

int muestra_menu() {
    cout << "\n===== " << config.nombre << " =====\n";
    cout << "1. Ver tabla\n2. Registrar partido\n3. Ver historial jornadas\n";
    cout << "4. Ver todos los partidos\n5. Exportar Tabla (Opcional)\n";
    cout << "6. Historial enfrentamientos (Opcional)\n7. Editar resultado (Opcional)\n8. Salir\nOpcion: ";
    int op;
    if (!(cin >> op)) { cin.clear(); string b; cin >> b; return 0; }
    return op;
}

// funcion main para ejecutar todas las funciones y que busque dentro de los archivos de fechas, partidos y config //
int main() {
    if (!lee_config("data/config.txt")) { cout << "Error configuracion.\n"; return 1; }
    
    int opcion = 0;
    while (opcion != 8) {
        opcion = muestra_menu();
        lee_partidos("data/partidos.txt");
        
        if (opcion == 1) { construye_tabla(); ordena_tabla(); muestra_tabla(); }
        else if (opcion == 2) { registrar_partido(); }
        else if (opcion == 3) { muestra_jornadas("data/fechas.txt"); }
        else if (opcion == 4) {
            for (int i = 0; i < total_partidos; i++) {
                cout << "J" << partidos[i].jornada << " | " << partidos[i].fecha << " | " 
                     << partidos[i].equipo_local << " " << partidos[i].goles_local << "-" 
                     << partidos[i].goles_visitante << " " << partidos[i].equipo_visitante << "\n";
            }
        }
        else if (opcion == 5) { construye_tabla(); ordena_tabla(); exportar_tabla(); }
        else if (opcion == 6) { historial_enfrentamientos(); }
        else if (opcion == 7) { editar_resultado(); }
    }
    return 0;
}
