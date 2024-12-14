#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct empleado {
    int codigo_empleado;
    string nombre_empleado;
    string apellido_empleado;
    int edad;
    string contrasenia;
    string correo;
    string telefono; 
};
struct cliente { //para archivo de texto
    int codigo_cliente;
    string tipo_cliente;
    int numero_visitas;
    double presupuesto;
    double total_gastado; //agregado
};

struct historial_compras { //para archivo de texto
    int codigo_ropa;
    int cantidad;
    double precio;
    int fecha; //dia
};

struct ropa {
    string codigo_ropa;
    enum categoria { deportiva, casual, formal, urbana, accesorios } categoria_ropa;
    string marca;
    string talla;
    double costo_unitario;
    double precio_unitario;
    int cantidad;
    enum temporada { verano, otono, invierno, primavera } temporada_ropa;
};


struct evento { // se tiene que hacer estrura de esto?
    string tipo;
    int duracion; //dias
    double impacto_ventas; //porcentaje incremento o decremento 
    double impacto_inventario; 
};
vector<empleado> registro_empleado;
vector<cliente> clientes;
vector<ropa> catalogo;
vector <evento> eventos;

// Declaración de funciones
void inicioSesionEmpleado(vector<empleado> &registro_empleado);
void registroEmpleado(vector<empleado> &registro_empleado);
void cargarEmpleados(vector<empleado> &registro_empleado);
void inicio();
void menu_Inicio(vector<empleado> &registro_empleado);
void imprimirMenuEmpleado(const empleado &empleado_activo);
void imprimirMenuEmpleado2(const cliente &cliente_activo);
void menuEmpleado(empleado &empleado_activo);
void SubmenuEmpleado(cliente cliente_activo);

// Primer Menu
void inicio() {
    cout << "--------------------------------------------" << endl;
    cout << "\tBienvenido a Tendencia Total" << endl;
    cout << "--------------------------------------------" << endl;
    cout << "\n\t\tMenu" << endl;
    cout << endl;
    cout << "\t  1.- Iniciar Sesion" << endl;
    cout << "\t  2.- Registrarse" << endl;
    cout << "\t  3.- Creditos" << endl;
    cout << "\t  4.- Salir" << endl;
    cout << "--------------------------------------------" << endl;
    cout << "\n\n\nOpcion: ";
}

void menu_Inicio(vector<empleado> &registro_empleado) {
    int opcion;
    do {
        inicio();
        cin >> opcion;
        switch (opcion) {
            case 1:
                inicioSesionEmpleado(registro_empleado);
                break;
            case 2:
                registroEmpleado(registro_empleado);
                break;
            case 3:
                cout << "Creditos" << endl;
                break;
            case 4:
                cout << "Gracias por usar Tendencia Total..." << endl;
                break;
            default:
                cout << "Opcion no valida" << endl;
                break;
        }
    } while (opcion != 4);
}

// Registro de empleados
void registroEmpleado(vector<empleado> &registro_empleado) {
    empleado empleado_aux;

    cout << "--------------------------------------------" << endl;
    cout << "\t Registrarse" << endl;
    cout << "--------------------------------------------" << endl;

    empleado_aux.codigo_empleado = registro_empleado.size() + 1;

    cin.ignore();
    cout << "\nIngrese su nombre: ";
    getline(cin, empleado_aux.nombre_empleado);

    cout << "Ingrese su apellido: ";
    getline(cin, empleado_aux.apellido_empleado);

    cout << "Ingrese su edad: ";
    cin >> empleado_aux.edad;

    cin.ignore();
    cout << "Ingrese una contrasenia: ";
    getline(cin, empleado_aux.contrasenia);

    cout << "Ingrese su correo: ";
    getline(cin, empleado_aux.correo);

    cout << "Ingrese su telefono (solo numeros): ";
    getline(cin, empleado_aux.telefono);

    registro_empleado.push_back(empleado_aux);

    ofstream archivo_empleado("registro_empleado.txt", ios::app);
    if (!archivo_empleado.is_open()) {
        cout << "No se pudo abrir el archivo para guardar el registro." << endl;
        return;
    }

    archivo_empleado << empleado_aux.codigo_empleado << ";"
                     << empleado_aux.nombre_empleado << ";"
                     << empleado_aux.apellido_empleado << ";"
                     << empleado_aux.edad << ";"
                     << empleado_aux.contrasenia << ";"
                     << empleado_aux.correo << ";"
                     << empleado_aux.telefono << "\n";

    archivo_empleado.close();

    cout << "\nRegistro Exitoso" << endl;
}

// Cargar empleados desde archivo
void cargarEmpleados(vector<empleado> &registro_empleado) {
    ifstream archivo_empleado("registro_empleado.txt");
    if (!archivo_empleado.is_open()) {
        return;
    }

    string linea;
    while (getline(archivo_empleado, linea)) {
        stringstream ss(linea);
        empleado empleado_aux;

        string codigo, edad;
        getline(ss, codigo, ';');
        getline(ss, empleado_aux.nombre_empleado, ';');
        getline(ss, empleado_aux.apellido_empleado, ';');
        getline(ss, edad, ';');
        getline(ss, empleado_aux.contrasenia, ';');
        getline(ss, empleado_aux.correo, ';');
        getline(ss, empleado_aux.telefono, ';');

        empleado_aux.codigo_empleado = stoi(codigo);
        empleado_aux.edad = stoi(edad);

        registro_empleado.push_back(empleado_aux);
    }

    archivo_empleado.close();
}

// Iniciar sesión de empleado
void inicioSesionEmpleado(vector<empleado> &registro_empleado) {
    cout << "--------------------------------------------" << endl;
    cout << "\t Iniciar Sesion" << endl;
    cout << "--------------------------------------------" << endl;

    string contrasenia;
    cout << "\nIngrese su contrasenia: ";
    cin.ignore();
    getline(cin, contrasenia);

    bool encontrado = false;
    for (auto &empleado_activo : registro_empleado) { // Cambiado a referencia modificable
        if (empleado_activo.contrasenia == contrasenia) {
            encontrado = true;

            // Llama al menú del empleado autenticado
            menuEmpleado(empleado_activo); // Ahora pasa la referencia correctamente
            break; 
        }
    }
    if (!encontrado) {
        cout << "Contraseña Incorrecta." << endl;
    }
}

void imprimirMenuEmpleado(const empleado &empleado_activo){
    cout << "--------------------------------------------" << endl;
    cout << "\tBienvenido " << empleado_activo.nombre_empleado << endl;
    cout << "\n\t\tMenu" << endl;
    cout << "--------------------------------------------" << endl;
    cout << "\t  1.- Ver Perfil" << endl;
    cout << "\t  2.- Instrucciones" << endl;
    cout << "\t  3.- Planificacion antes de la jornada" << endl;
    //cout << "\t\t  3.1.- Ver catalogo" << endl;
    //cout << "\t\t  3.2.- Ver Stock" << endl;
    //cout << "\t\t  3.3.- Comprar Articulos" << endl;
    //cout << "\t\t  3.4.- Colocar Articulos" << endl;
    //cout << "\t\t  3.5.- Mis Pedidos" << endl;
    cout << "\t  4.- Ver Historial de Compras" << endl;
    cout << "\t  5.- Iniciar Jornada" << endl;
    cout << "\t  6.- Cerrar Sesion" << endl;
    cout << "--------------------------------------------" << endl;
    cout << "\n\n\nOpcion: ";
}void imprimirMenuSubmenu() {
    cout << "--------------------------------------------" << endl;
    cout << "\tPlanificación antes de la jornada" << endl;
    cout << "--------------------------------------------" << endl;
    cout << "\t  1.- Ver catálogo" << endl;
    cout << "\t  2.- Ver stock" << endl;
    cout << "\t  3.- Comprar artículos" << endl;
    cout << "\t  4.- Colocar artículos" << endl;
    cout << "\t  5.- Mis pedidos" << endl;
    cout << "\t  6.- Volver al menú principal" << endl;
    cout << "--------------------------------------------" << endl;
    cout << "\n\n\nOpción: ";
}

void SubmenuEmpleado(empleado empleado_activo) {
    int opcion;
    do {
        imprimirMenuSubmenu();
        cin >> opcion;
        switch (opcion) {
            case 1:
                cout << "Ver catálogo" << endl;
                // Agregar lógica específica aquí
                break;
            case 2:
                cout << "Ver stock" << endl;
                // Agregar lógica específica aquí
                break;
            case 3:
                cout << "Comprar artículos" << endl;
                // Agregar lógica específica aquí
                break;
            case 4:
                cout << "Colocar artículos" << endl;
                // Agregar lógica específica aquí
                break;
            case 5:
                cout << "Mis pedidos" << endl;
                // Agregar lógica específica aquí
                break;
            case 6:
                cout << "Volviendo al menú principal..." << endl;
                break;
            default:
                cout << "Opción no válida." << endl;
                break;
        }
    } while (opcion != 6);
}

void menuEmpleado(empleado &empleado_activo) {
    int opcion;
    do {
        imprimirMenuEmpleado(empleado_activo);
        cin >> opcion;
        switch (opcion) {
            case 1:
                cout << "\n\n--------------------------------------------" << endl;
                cout << "\t\tPerfil" << endl;
                cout << "--------------------------------------------" << endl;
                cout << "\t\tNombre: " << empleado_activo.nombre_empleado << endl;
                cout << "\t\tApellido: " << empleado_activo.apellido_empleado << endl;
                cout << "\t\tEdad: " << empleado_activo.edad << endl;
                cout << "\t\tCorreo: " << empleado_activo.correo << endl;
                cout << "\t\tTeléfono: " << empleado_activo.telefono << endl;
                cout << "\n" << endl;
                break;
            case 2:
                cout << "Instrucciones" << endl;
                break;
            case 3:
                cout << "Planificación antes de la jornada" << endl;
                SubmenuEmpleado(empleado_activo); // Llamada corregida
                break;
            case 4:
                cout << "Ver historial de compras" << endl;
                break;
            case 5:
                cout << "Iniciar jornada" << endl;
                break;
            case 6:
                cout << "Cerrando sesión..." << endl;
                break;
            default:
                cout << "Opción no válida." << endl;
                break;
        }
    } while (opcion != 6);
}

// Función principal
int main() {
    cargarEmpleados(registro_empleado);
    menu_Inicio(registro_empleado);
    return 0;
}
