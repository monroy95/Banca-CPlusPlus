/**
  * @file main.cpp
  * @version 1
  * @author Mario Monroy Canizales
  */

#include <iostream>
#include <windows.h>
#include <locale>
#include <fstream>

#define ARRIBA 'w'
#define ABAJO 's'
#define ENTER 13

const int SIZEREG = 10;
const int SIZEMOVIMIENTOS = 10;

using namespace std;

// Definicion de estructura
struct CuentaRegistro
{
    float movimientos[20][3]; // 15 filas y 3 columnas
    int index;
    int no_cuenta;
    char nombre_completo[45];
    char dpi[20];
    char nit[15];
    float saldo_cuenta;
};

/**
 * Prototipos para mostrar opciones de menu
 */

/**
 * @brief menu_principal procedimiento generador de menu principal
 */
void menu_principal();

/**
 * @brief ingreso_nueva_cuenta procedimiento para crear cuentas bancarias
 */
void ingreso_nueva_cuenta();

/**
 * @brief movimientos_bancarios procedimiento para ver detalles de movimientos ingresos/gastos
 * y detalles de cuentas
 */
void movimientos_bancarios();

/**
 * @brief registrar_movimiento procedimiento para registrar movimientos ingresos/gastos
 */
void registrar_movimiento();

/**
 * @brief menu funcion para generar menu y registrar movimientos de arriba y abajo
 */
int menu(const char titulo[], const char *opciones[], int n);


// API DE WINDOWS
/**
 * Funciones para permitir movimiento de teclas arriba/abajo
 */
char getch2()
{
    char c = 0;
    DWORD modo, contador;
    HANDLE ih = GetStdHandle(STD_INPUT_HANDLE);

    // Desactivamos escritura en terminal
    SetConsoleMode(ih, modo & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));

    ReadConsoleA(ih, &c, 1, &contador, NULL);

    if (c == 0)
    {
        ReadConsoleA(ih, &c, 1, &contador, NULL);
    }

    SetConsoleMode(ih, modo); // Devolvemos control normal

    return c;
}

/**
 * Funcion para manipular la posicion de seleccion
 */
int gotoxy(int x, int y)
{
    COORD cp = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cp);
}


/**
 * Punto de ejecucion
 */
int main()
{
    setlocale(LC_ALL, ""); // Para mostrar acentos
    menu_principal();

    return 0;
}

void menu_principal()
{
    bool repetir = true;
    int opcion;

    // Titulo del men? y nombres de las opciones
    const char *titulo = "MENU INICIO";
    const char *opciones[] = {
        "Ingresar nueva cuenta bancaria",
        "Registrar movimiento",
        "Ver movimientos bancarios",
        "Salir",
    };

    int n = 4; // Numero de opciones

    do
    {
        opcion = menu(titulo, opciones, n);

        switch (opcion)
        {
        case 1:
            ingreso_nueva_cuenta();
            break;

        case 2:
            registrar_movimiento();
            break;

        case 3:
            movimientos_bancarios();
            break;

        case 4:
            cout << "\nSesi?n cerrada" << endl;
            repetir = false;
            system("pause>nul");
            break;
        }

    } while (repetir);
}

void ingreso_nueva_cuenta()
{
    bool repetir = true;
    int opcion;
    fstream f;
    int count = 0;
    int indice = 0;

    // Lectura de archivos: para agregar registros mas lo ya existentes
    // Si encuentra un archivo con datos los cargara a memoria para volver a reutilizarlos
    CuentaRegistro registros[SIZEREG];

    f.open("records.txt", ios::in);
    if (f.is_open())
    {
        // Se cargan los datos a una estructura copia
        f.read(reinterpret_cast<char *>(registros), SIZEREG * sizeof(CuentaRegistro));
        f.close();
    }
    else
    {
        cout << "\nERROR LECTURA\n";
    }

    // Titulo del menu y nombres de las opciones
    const char *titulo = "INGRESO CUENTA BANCARIA";
    const char *opciones[] = {"Crear Cuenta", "Regresar"};
    int n = 2; // Numero de opciones

    do
    {
        // Genera el menu
        opcion = menu(titulo, opciones, n);

        system("cls"); // limpia la pantalla
        switch (opcion)
        {
        case 1: // Registro de cuenta
            cout << "\n REGISTRO DE CUENTA NUEVA\n";
            // Contador de elementos: busca posiciones disponibles para insertar datos
            for (int i = 0; i < SIZEREG; i++)
            {
                if (registros[i].index == 111)
                    count++;
            }
            indice = count;

            cout << "Nombre Completo: ";
            cin.getline(registros[indice].nombre_completo, 45);
            cin.ignore();

            cout << "DPI: ";
            cin.getline(registros[indice].dpi, 20);
            cin.ignore();

            cout << "NIT: ";
            cin.getline(registros[indice].nit, 15);
            cin.ignore();

            registros[indice].no_cuenta = count + 1;
            registros[indice].index = 111;

            cout << endl;
            cout << "------------------------------------------------------------" << endl;
            cout << "CUENTA CREADA CON EXITO - DETALLES" << endl;
            cout << "No. Cuenta: " << registros[indice].no_cuenta << endl;
            cout << "Nombre cuentahabiente: " << registros[indice].nombre_completo << endl;
            cout << "DPI: " << registros[indice].dpi << endl;
            cout << "NIT: " << registros[indice].nit << endl;
            cout << "------------------------------------------------------------" << endl;
            cout << endl;
            cout << "Presione ENTER para continuar\n";

            // ESCRITURA DE REGISTROS
            f.open("records.txt", ios::out);

            if (f.is_open())
            {
                f.write(reinterpret_cast<char *>(registros), SIZEREG * sizeof(CuentaRegistro));
                f.close();
            }
            else
            {
                cout << "\nERROR ESCRITURA\n";
            }

            // debug: mostrando los registros guardados
            //            for (int i = 0; i < SIZEREG; i++)
            //            {
            //                if (registros[i].index == 111)
            //                {
            //                    cout << endl;
            //                    cout << "------------------------------------------------------------" << endl;
            //                    cout << "No. Cuenta: " << registros[i].no_cuenta << endl;
            //                    cout << "Nombre cuentahabiente: " << registros[i].nombre_completo << endl;
            //                    cout << "DPI: " << registros[i].dpi << endl;
            //                    cout << "NIT: " << registros[i].nit << endl;
            //                    cout << "------------------------------------------------------------" << endl;
            //                    cout << endl;
            //                }
            //            }
            cout << "Datos guardados...";
            system("pause>nul");
            break;

        case 2:
            repetir = false;
            break;
        }

    } while (repetir);
}

void movimientos_bancarios()
{
    bool repetir = true;
    int opcion, num_cuenta;
    fstream f;
    float total_cuenta = 0;

    // Titulo del men? y nombres de las opciones
    const char *titulo = "MOVIMIENTOS BANCARIOS";
    const char *opciones[] = {"Ver detalles de cuenta", "Ver todas las cuentas", "Regresar"};
    int n = 3; // Numero de opciones

    // Lectura de archivos: para registrar movimiento en X cuenta
    CuentaRegistro registros[SIZEREG];

    f.open("records.txt", ios::in);
    if (f.is_open())
    {
        f.read(reinterpret_cast<char *>(registros), SIZEREG * sizeof(CuentaRegistro));
        f.close();
    }
    else
    {
        cout << "\nERROR LECTURA\n";
    }

    do
    {
        opcion = menu(titulo, opciones, n);

        system("cls");
        switch (opcion)
        {
        case 1:
            cout << "\nDETALLES DE MOVIMIENTOS\n"
                 << endl;
            cout << "Ingrese el n?mero de cuenta a consultar: " << endl;
            cin >> num_cuenta;

            for (int i = 0; i < SIZEREG; i++)
            {
                if (registros[i].index == 111 && registros[i].no_cuenta == num_cuenta)
                {
                    cout << endl;
                    cout << "------------------------------------------------------------" << endl;
                    cout << "DETALLES DE LA CUENTA" << endl;
                    cout << "No. Cuenta: " << registros[i].no_cuenta << endl;
                    cout << "Nombre cuentahabiente: " << registros[i].nombre_completo << endl;
                    cout << "DPI: " << registros[i].dpi << endl;
                    cout << "NIT: " << registros[i].nit << endl;
                    cout << endl;

                    cout << "MOVIMIENTOS BANCARIOS" << endl;
                    for (int n = 0; n < 20; ++n)
                    {
                        if (registros[i].movimientos[n][1] == 1)
                        {
                            cout << "MONTO INGRESO: "
                                 << "GTQ " << registros[i].movimientos[n][2] << endl;
                            total_cuenta += (registros[i].movimientos[n][2]);
                        }
                        if (registros[i].movimientos[n][1] == 2)
                        {
                            cout << "MONTO GASTO:   "
                                 << "GTQ " << registros[i].movimientos[n][2] << endl;
                            total_cuenta -= (registros[i].movimientos[n][2]);
                        }
                    }
                    cout << endl;
                    cout << "TOTAL EN LA CUENTA: "
                         << "GTQ " << total_cuenta << endl;
                    cout << endl;

                    cout << "------------------------------------------------------------" << endl;
                    cout << endl;
                    cout << endl;

					total_cuenta = 0;
                    system("pause>nul");
                    break;
                }
                else
                {
                    cout << "El n?mero de cuenta no fue encontrado" << endl;
                }
            }

            system("pause>nul");
            break;

        case 2:
            cout << "\nDETALLES DE TODAS LAS CUENTAS\n" << endl;
            total_cuenta = 0;

            for (int i = 0; i < SIZEREG; i++)
            {
                if (registros[i].index == 111) // Si hay un registro valido
                {
                	total_cuenta = 0;
                    cout << endl;
                    cout << "------------------------------------------------------------" << endl;
                    cout << "DETALLES DE LA CUENTA" << endl;
                    cout << "No. Cuenta: " << registros[i].no_cuenta << endl;
                    cout << "Nombre cuentahabiente: " << registros[i].nombre_completo << endl;
                    cout << "DPI: " << registros[i].dpi << endl;
                    cout << "NIT: " << registros[i].nit << endl;
                    cout << endl;

                    cout << "MOVIMIENTOS BANCARIOS" << endl;
                    for (int n = 0; n < 20; ++n)
                    {
                        if (registros[i].movimientos[n][1] == 1)
                        {
                            cout << "MONTO INGRESO: "
                                 << "GTQ " << registros[i].movimientos[n][2] << endl;
                            total_cuenta += (registros[i].movimientos[n][2]);
                        }
                        if (registros[i].movimientos[n][1] == 2)
                        {
                            cout << "MONTO GASTO:   "
                                 << "GTQ " << registros[i].movimientos[n][2] << endl;
                            total_cuenta -= (registros[i].movimientos[n][2]);
                        }
                    }
                    cout << endl;
                    cout << "TOTAL EN LA CUENTA: "
                         << "GTQ " << total_cuenta << endl;
                    cout << endl;

                    cout << "------------------------------------------------------------" << endl;
                    cout << endl;
                    cout << endl;
                }
            }

			total_cuenta = 0;
            system("pause>nul");
            break;

        case 3:
            cout << "Regresando..." << endl;
            repetir = false;
            break;
        }

    } while (repetir);
}

void registrar_movimiento()
{
    bool repetir = true;
    int opcion, num_cuenta, contador = 0, indicador;
    fstream f;
    float mont;

    // Titulo del menu y nombres de las opciones
    const char *titulo = "REGISTRO DE MOVIMIENTOS BANCARIOS";
    const char *opciones[] = {"Registrar Ingreso", "Registrar Retiro", "Regresar"};
    int n = 3; // Numero de opciones

    // Lectura de archivos: para registrar movimiento en X cuenta
    CuentaRegistro registros[SIZEREG];

    f.open("records.txt", ios::in);
    if (f.is_open())
    {
        f.read(reinterpret_cast<char *>(registros), SIZEREG * sizeof(CuentaRegistro));
        f.close();
    }
    else
    {
        cout << "\nERROR LECTURA\n";
    }

    do
    {
        opcion = menu(titulo, opciones, n);

        system("cls");
        switch (opcion)
        {
        case 1:
            cout << "\nREGISTRO DE INGRESOS\n";
            cout << "Ingrese el n?mero de cuenta al que desea acreditar: " << endl;
            cin >> num_cuenta;

            for (int i = 0; i < SIZEREG; i++)
            {
                // Si existe la cuenta que se esta consultando
                if (registros[i].index == 111 && registros[i].no_cuenta == num_cuenta)
                {
                    cout << "Numero de cuenta encontrado!\n" << endl;
                    // Busqueda de movs: Busca en la matriz de movimiento el valor 111, y en
                    // base al resultado se autoincrementa el indica para asginar valor
                    for (int z = 0; z < 5; ++z)
                    {
                        if (registros[i].movimientos[z][0] == 111)
                        {
                            contador++; // Si se encuentra
                        }
                    }
                    indicador = contador;

                    cout << "\nIngrese el monto: " << endl;
                    cin >> registros[i].movimientos[indicador][2];

                    registros[i].movimientos[indicador][0] = 111;
                    registros[i].movimientos[indicador][1] = 1; // con 1 es ingreso

                    //					DEBUG
                    //					cout << "Mostrando resultados" << endl;
                    //				    for (int n = 0; n < 5; ++n) {
                    //				        for (int j = 0; j < 3; ++j) {
                    //				            cout << " | " << registros[i].movimientos[n][j] << " | ";
                    //				        }
                    //				        cout << endl;
                    //				    }
                    system("pause>nul");
                    break; // si se encuentra la cuenta y se agregar el monto se rompe el ciclo
                }
                else
                {
                    cout << "El n?mero de cuenta no fue encontrado" << endl;
                }
            }
            cout << "Datos guardados..." << endl;

            // ESCRITURA DE REGISTROS
            f.open("records.txt", ios::out);

            if (f.is_open())
            {
                f.write(reinterpret_cast<char *>(registros), SIZEREG * sizeof(CuentaRegistro));
                f.close();
            }
            else
            {
                cout << "\nERROR ESCRITURA\n";
            }

            system("pause>nul");
            break;

        case 2:
            cout << "\nREGISTRO DE GASTOS\n";
            cout << "Ingrese el n?mero de cuenta al que desea debitar: " << endl;
            cin >> num_cuenta;

            for (int i = 0; i < SIZEREG; i++)
            {
                // Si existe la cuenta que se esta consultando
                if (registros[i].index == 111 && registros[i].no_cuenta == num_cuenta)
                {
                    cout << "Numero de cuenta encontrado!\n" << endl;
                    // Busqueda de movs: Busca en la matriz de movimiento el valor 111, y en
                    // base al resultado se autoincrementa el indica para asginar valor
                    for (int z = 0; z < 5; ++z)
                    {
                        if (registros[i].movimientos[z][0] == 111)
                        {
                            contador++; // Si se encuentra
                        }
                    }
                    indicador = contador;

                    cout << "Ingrese el monto: " << endl;
                    cin >> registros[i].movimientos[indicador][2];

                    registros[i].movimientos[indicador][0] = 111;
                    registros[i].movimientos[indicador][1] = 2; // con el valor 2 es ggasto
                    // DEBUG
                    //					cout << "Mostrando resultados" << endl;
                    //				    for (int n = 0; n < 5; ++n) {
                    //				        for (int j = 0; j < 3; ++j) {
                    //				            cout << " | " << registros[i].movimientos[n][j] << " | ";
                    //				        }
                    //				        cout << endl;
                    //				    }
                    system("pause>nul");
                    break; // si se encuentra la cuenta y se agregar el monto se rompe el ciclo
                }
                else
                {
                    cout << "El n?mero de cuenta no fue encontrado" << endl;
                }
            }
            cout << "Datos guardados..." << endl;

            // ESCRITURA DE REGISTROS
            f.open("records.txt", ios::out);

            if (f.is_open())
            {
                f.write(reinterpret_cast<char *>(registros), SIZEREG * sizeof(CuentaRegistro));
                f.close();
            }
            else
            {
                cout << "\nERROR ESCRITURA\n";
            }

            system("pause>nul");
            break;

        case 3:
            repetir = false;
            break;
        }

    } while (repetir);
}

int menu(const char titulo[], const char *opciones[], int n)
{
    int opcionSeleccionada = 1; // Indica la opcionSeleccionada

    int tecla;

    bool repetir = true; // controla el bucle para regresar a la rutina que lo llamo, al presionar ENTER

    do
    {
        system("cls");
        // system("color 1e");
        gotoxy(5, 3 + opcionSeleccionada);
        cout << ">>" << endl;

        // Imprime el t?tulo del men?
        gotoxy(15, 2);
        cout << titulo;

        // Imprime las opciones del men?
        for (int i = 0; i < n; ++i)
        {
            gotoxy(10, (4 + i));
            // cout << i + 1 << " -" << opciones[i];
            cout << "- " << opciones[i];
        }

        // Solo permite que se ingrese ARRIBA, ABAJO o ENTER

        do
        {
            tecla = getch2();
            // gotoxy(15, 15); cout << "tecla presionada: " << (char)tecla << " = " << tecla;
        } while (tecla != ARRIBA && tecla != ABAJO && tecla != ENTER);

        switch (tecla)
        {

        case ARRIBA: // En caso que se presione ARRIBA

            opcionSeleccionada--;

            if (opcionSeleccionada < 1)
            {
                opcionSeleccionada = n;
            }

            break;

        case ABAJO:
            opcionSeleccionada++;

            if (opcionSeleccionada > n)
            {
                opcionSeleccionada = 1;
            }

            break;

        case ENTER:
            repetir = false;
            break;
        }

    } while (repetir);

    return opcionSeleccionada;
}

