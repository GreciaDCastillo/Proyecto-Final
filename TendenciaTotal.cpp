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

struct catalogo_ropa {
    string codigo_ropa;
    string categoria;
    string marca;
    string talla;
    double precio_unitario;
    int cantidad; // Cantidad disponible en el catálogo.
    string temporada;
};

struct inventario_ropa {
    string codigo_ropa;
    string categoria;
    string marca;
    string talla;
    double costo_unitario;
    int cantidad; // Cantidad en el inventario general.
    string temporada;
};


struct pedidos { //para archivo de texto
    string codigo_pedido; //codigo ropa
    int cantidad;
    double total;
};

struct evento { // se tiene que hacer estrura de esto?
    string tipo;
    int duracion; //dias
    double impacto_ventas; //porcentaje incremento o decremento 
    double impacto_inventario; 
};
vector<empleado> registro_empleado;
vector<cliente> clientes;
vector<catalogo_ropa> catalogo;
vector<inventario_ropa> inventario;
vector <evento> eventos;
vector <pedidos> ventas;

// Declaración de funciones
void inicioSesionEmpleado(vector<empleado> &registro_empleado);
void registroEmpleado(vector<empleado> &registro_empleado);
void cargarEmpleados(vector<empleado> &registro_empleado);
void inicio();
void menu_Inicio(vector<empleado> &registro_empleado);
void imprimirMenuEmpleado(const empleado &empleado_activo);
void imprimirMenuSubmenu();
void menuEmpleado(empleado &empleado_activo);
void SubmenuEmpleado(vector<catalogo_ropa> catalogo, vector<inventario_ropa> inventario, vector<pedidos> ventas);
void cargarCatalogo(vector<catalogo_ropa> &catalogo);
void verCatalogo(vector<catalogo_ropa> &catalogo);
void guardarCatalogo(vector<catalogo_ropa> &catalogo);
void guardarInventario(vector<inventario_ropa> &catalogo);
void verInventario(vector<inventario_ropa> &catalogo);
void cargarInventario(vector<inventario_ropa> &catalogo);
void comprarArticulos(vector<inventario_ropa> &inventario, vector<pedidos> &ventas, vector<catalogo_ropa> &catalogo);
void colocarArticulos(vector<catalogo_ropa> &catalogo, vector<pedidos> &ventas, vector<inventario_ropa> &inventario);

// Función principal
int main() {
    cargarEmpleados(registro_empleado);
    cargarInventario(inventario);
    cargarCatalogo(catalogo);
    menu_Inicio(registro_empleado);
    return 0;
}

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

    ofstream archivo_empleado("registro_empleado.txt", ios::out | ios::app);
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
    ifstream archivo_empleado("registro_empleado.txt", ios::in);
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
    int intentos = 3;

    while (intentos > 0) {
        cout << "\nIngrese su contrasenia (Intentos restantes: " << intentos << "): ";
        cin.ignore();
        getline(cin, contrasenia);

        for (auto &empleado_activo : registro_empleado) {
            if (empleado_activo.contrasenia == contrasenia) {
                cout << "Bienvenido " << empleado_activo.nombre_empleado << "!" << endl;
                menuEmpleado(empleado_activo);
                return;
            }
        }
        cout << "Contraseña Incorrecta." << endl;
        intentos--;
    }

    if (intentos == 0) {
        cout << "Demasiados intentos fallidos. Regresando al menú principal." << endl;
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
    cout << "\t  4.- Ver Historial de Compras" << endl;
    cout << "\t  5.- Iniciar Jornada" << endl;
    cout << "\t  6.- Cerrar Sesion" << endl;
    cout << "--------------------------------------------" << endl;
    cout << "\n\n\nOpcion: ";
}
void imprimirMenuSubmenu() {
    cout << "--------------------------------------------" << endl;
    cout << "\tPlanificación antes de la jornada" << endl;
    cout << "--------------------------------------------" << endl;
    cout << "\t  1.- Ver catálogo" << endl;
    cout << "\t  2.- Ver Inventario" << endl;
    cout << "\t  3.- Comprar artículos" << endl;
    cout << "\t  4.- Colocar artículos en el catalogo" << endl;
    cout << "\t  5.- Historial de pedidos" << endl;
    cout << "\t  6.- Volver al menú principal" << endl;
    cout << "--------------------------------------------" << endl;
    cout << "\n\n\nOpción: ";
}

void SubmenuEmpleado(vector<catalogo_ropa> catalogo, vector<inventario_ropa> inventario, vector<pedidos> ventas) {
    int opcion;
    do {
        imprimirMenuSubmenu();
        cin >> opcion;
        switch (opcion) {
            case 1:
                cout << "Ver catálogo" << endl;
                verCatalogo(catalogo);
                break;
            case 2:
                cout << "Ver Inventario" << endl;
                verInventario(inventario);
                break;
            case 3:
                cout << "Comprar artículos" << endl;
                comprarArticulos(inventario, ventas, catalogo);
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
                SubmenuEmpleado(catalogo, inventario, ventas); // Llamada a la función SubmenuEmpleado
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

#include <fstream>
#include <sstream>

void verCatalogo(const vector<catalogo_ropa> &catalogo) {
    cout << "\n--------------------------------------------" << endl;
    cout << "\t\tVer catálogo" << endl;
    cout << "--------------------------------------------" << endl;

    if (catalogo.empty()) {
        cout << "El catálogo se encuentra vacío." << endl;
        return;
    }

    for (const auto &articulo : catalogo) {
        cout << "Código: " << articulo.codigo_ropa << endl;
        cout << "Categoría: " << articulo.categoria << endl;
        cout << "Marca: " << articulo.marca << endl;
        cout << "Talla: " << articulo.talla << endl;
        cout << "Precio Unitario: $" << articulo.precio_unitario << endl;
        cout << "Cantidad Disponible: " << articulo.cantidad << endl;
        cout << "Temporada: " << articulo.temporada << endl;
        cout << "--------------------------------------------" << endl;
    }
}

void verInventario(const vector<inventario_ropa> &inventario) {
    cout << "\n--------------------------------------------" << endl;
    cout << "\t\tInventario" << endl;
    cout << "--------------------------------------------" << endl;

    if (inventario.empty()) {
        cout << "El inventario se encuentra vacío." << endl;
        return;
    }

    for (const auto &articulo : inventario) {
        cout << "Código: " << articulo.codigo_ropa << endl;
        cout << "Categoría: " << articulo.categoria << endl;
        cout << "Marca: " << articulo.marca << endl;
        cout << "Talla: " << articulo.talla << endl;
        cout << "Costo Unitario: $" << articulo.costo_unitario << endl;
        cout << "Cantidad en Stock: " << articulo.cantidad << endl;
        cout << "Temporada: " << articulo.temporada << endl;
        cout << "--------------------------------------------" << endl;        
    }
}

// Leer catálogo desde archivo
void cargarCatalogo(vector<catalogo_ropa> &catalogo) {
    ifstream archivo_catalogo("catalogo.txt", ios::in);
    
    if (!archivo_catalogo.is_open()) {
        cout << "Error al abrir el archivo de catálogo." << endl;
        return;
    }

    string linea;
    while (getline(archivo_catalogo, linea)) {
        stringstream ss(linea);
        catalogo_ropa articulo;
        string precio, cantidad;

        getline(ss, articulo.codigo_ropa, ';');
        getline(ss, articulo.categoria, ';');
        getline(ss, articulo.marca, ';');
        getline(ss, articulo.talla, ';');
        getline(ss, precio, ';');
        getline(ss, cantidad, ';');
        getline(ss, articulo.temporada, ';');

        articulo.precio_unitario = stod(precio);
        articulo.cantidad = stoi(cantidad);

        catalogo.push_back(articulo);
    }
    archivo_catalogo.close();
}

void guardarCatalogo(const vector<catalogo_ropa> &catalogo) {
    ofstream archivo_catalogo("catalogo.txt");
    if (!archivo_catalogo.is_open()) {
        cout << "Error al abrir el archivo de catálogo." << endl;
        return;
    }

    for (const auto &articulo : catalogo) {
        archivo_catalogo << articulo.codigo_ropa << ";"
                         << articulo.categoria << ";"
                         << articulo.marca << ";"
                         << articulo.talla << ";"
                         << articulo.precio_unitario << ";"
                         << articulo.cantidad << ";"
                         << articulo.temporada << endl;
    }
    archivo_catalogo.close();
    }

// Leer inventario desde archivo
void cargarInventario(vector<inventario_ropa> &inventario) {
    ifstream archivo_inventario("inventario.txt");
    if (!archivo_inventario.is_open()) {
        cout << "Error al abrir el archivo de inventario." << endl;
        return;
    }

    string linea;
    while (getline(archivo_inventario, linea)) {
        stringstream ss(linea);
        inventario_ropa articulo;
        string costo, cantidad;

        getline(ss, articulo.codigo_ropa, ';');
        getline(ss, articulo.categoria, ';');
        getline(ss, articulo.marca, ';');
        getline(ss, articulo.talla, ';');
        getline(ss, costo, ';');
        getline(ss, cantidad, ';');
        getline(ss, articulo.temporada, ';');

        try {
            articulo.costo_unitario = stod(costo);
            articulo.cantidad = stoi(cantidad);
        } catch (...) {
            cout << "Error al leer línea del inventario: " << linea << endl;
            continue;
        }

        inventario.push_back(articulo);
    }

    archivo_inventario.close();
}


void guardarInventario(const vector<inventario_ropa> &inventario) {
    ofstream archivo_inventario("inventario.txt");
    if (!archivo_inventario.is_open()) {
        cout << "Error al abrir el archivo de inventario." << endl;
        return;
    }

    for (const auto &articulo : inventario) {
        if (articulo.codigo_ropa.empty()) {
            cout << "Error: Código vacío encontrado en inventario." << endl;
            continue;
        }

        archivo_inventario << articulo.codigo_ropa << ";"
                           << articulo.categoria << ";"
                           << articulo.marca << ";"
                           << articulo.talla << ";"
                           << articulo.costo_unitario << ";"
                           << articulo.cantidad << ";"
                           << articulo.temporada << endl;
    }

    archivo_inventario.close();
}

void comprarArticulos(vector<inventario_ropa> &inventario, vector<pedidos> &ventas, vector<catalogo_ropa> &catalogo) {
    pedidos nuevo_pedido;
    bool encontrado = false;

    cout << "--------------------------------------------" << endl;
    cout << "Comprar artículos" << endl;
    cout << "--------------------------------------------" << endl;

    cout << "Ingrese el código de la prenda que desea comprar: ";
    cin.ignore();
    getline(cin, nuevo_pedido.codigo_pedido);

    for (auto &articulo_catalogo : catalogo) {
        if (articulo_catalogo.codigo_ropa == nuevo_pedido.codigo_pedido) {
            encontrado = true;

            cout << "Ingrese la cantidad de prendas que desea comprar: ";
            cin >> nuevo_pedido.cantidad;

            if (nuevo_pedido.cantidad <= 0) {
                cout << "Cantidad inválida. Compra cancelada." << endl;
                return;
            }

            nuevo_pedido.total = articulo_catalogo.precio_unitario * nuevo_pedido.cantidad;

            // Actualizar inventario
            for (auto &articulo_inventario : inventario) {
                if (articulo_inventario.codigo_ropa == nuevo_pedido.codigo_pedido) {
                    articulo_inventario.cantidad += nuevo_pedido.cantidad;
                    break;
                }
            }

            ventas.push_back(nuevo_pedido);
            //guardarInventario(inventario);

            ofstream archivo_pedidos("pedidos.txt", ios::app);
            if (archivo_pedidos.is_open()) {
                archivo_pedidos << nuevo_pedido.codigo_pedido << ";"
                                << nuevo_pedido.cantidad << ";"
                                << nuevo_pedido.total << endl;
                archivo_pedidos.close();
            } else {
                cout << "Error al guardar el pedido en el archivo." << endl;
            }

            cout << "Artículos comprados exitosamente." << endl;
            return;
        }
    }

    if (!encontrado) {
        cout << "Prenda no encontrada en el catálogo." << endl;
    }
}