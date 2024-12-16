#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

 
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
    double total_gastado;
    int total_productos; //agregado
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
    int cantidad_vendida;
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

struct compra_inventario {
    string codigo_ropa;
    string categoria;
    string marca;
    string talla;
    double costo_unitario; // Costo del artículo comprado.
    int cantidad;
    double total_costo; // costo_unitario * cantidad
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
vector<compra_inventario> historial_compras;
vector<pedidos> compras;
vector <evento> eventos;

double presupuesto = 25000.0; // presupuesto inicial
int contador_jornadas = 0; // contador de dias de jornada

// Declaración de funciones
void inicioSesionEmpleado(vector<empleado> &registro_empleado);
void registroEmpleado(vector<empleado> &registro_empleado);
void cargarEmpleados(vector<empleado> &registro_empleado);
void inicio();
void menu_Inicio(vector<empleado> &registro_empleado);
void imprimirMenuEmpleado(const empleado &empleado_activo);
void menuEmpleado(empleado &empleado_activo);
void SubmenuEmpleado();
void menuFinJornada();
void CicloDiario(vector<cliente> &clientes, vector<catalogo_ropa> &catalogo);
void VerCatalogo(vector<catalogo_ropa> catalogo);
void VerInventario(vector<inventario_ropa> inventario);
void cargarInventario(vector<inventario_ropa> &inventario);
void guardarInventario(const vector<inventario_ropa> &inventario);
void HacerPedido(vector<inventario_ropa> &inventario, vector<compra_inventario> &historial_compras, double &presupuesto);
void guardarHistorialCompras(const vector<compra_inventario> &historial_compras);
void VerHistorialCompras();
void AgregarCatalogo(vector<catalogo_ropa> &catalogo, vector<inventario_ropa> &inventario);
void GenerarCliente(vector<cliente> &clientes, int cantidad_clientes);
void GenerarEvento(evento &nuevoEvento);

void SimularCompras(vector<cliente> &clientes, vector<catalogo_ropa> &catalogo, evento eventoDelDia);
void verReportes(const vector<cliente> &clientes,vector<catalogo_ropa> &catalogo);

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
    cout << "\nOpcion: ";
}

void menu_Inicio(vector<empleado> &registro_empleado) {
    int opcion;
    do {
        inicio();
        cin >> opcion;
        switch (opcion) {
            case 1:
                inicioSesionEmpleado(registro_empleado);
                system("pause");
                break;
            case 2:
                registroEmpleado(registro_empleado);
                system("pause");
                break;
            case 3:
                cout << "Creditos" << endl;
                system("pause");
                break;
            case 4:
                cout << "Gracias por usar Tendencia Total..." << endl;
                system("pause");
                break;
            default:
                cout << "Opcion no valida" << endl;
                system("pause");
                break;
        }
    } while (opcion != 4);
}

// Registro de empleados
void registroEmpleado(vector<empleado> &registro_empleado) {
    empleado empleado_aux;

    cout << "--------------------------------------------" << endl;
    cout << "\t \t Registrarse" << endl;
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

void VerCatalogo(vector<catalogo_ropa> catalogo){
    cout << "--------------------------------------------" << endl;
    cout << "\t Ver Catalogo" << endl;
    cout << "--------------------------------------------" << endl;

    if (catalogo.empty()) {
        cout << "\n\nEl catalogo esta vacio" << endl;
        return;
    }

    for (auto &catalogo_ropa : catalogo) {
        cout << "\nCodigo: " << catalogo_ropa.codigo_ropa << endl;
        cout << "Categoria: " << catalogo_ropa.categoria << endl;
        cout << "Marca: " << catalogo_ropa.marca << endl;
        cout << "Talla: " << catalogo_ropa.talla << endl;
        cout << "Precio Unitaraio: " << catalogo_ropa.precio_unitario << endl;
        cout << "Cantidad Disponible: " << catalogo_ropa.cantidad << endl;
        cout << "Temporada: " << catalogo_ropa.temporada << endl;
        cout << "--------------------------------------------" << endl;
    }
}

void AgregarCatalogo(vector<catalogo_ropa> &catalogo, vector<inventario_ropa> &inventario) {
    string codigo;
    int cantidad;

    cout << "\nIngrese el codigo del producto a agregar al catalogo: ";
    cin >> codigo;
    cout << "Ingrese la cantidad a transferir: ";
    cin >> cantidad;

    for (auto &item : inventario) {
        if (item.codigo_ropa == codigo) {
            if (item.cantidad >= cantidad) {
                catalogo_ropa nuevo = {item.codigo_ropa, item.categoria, item.marca, 
                                       item.talla, item.costo_unitario * 1.5, 
                                       cantidad, item.temporada};
                catalogo.push_back(nuevo);
                item.cantidad -= cantidad;
                cout << "Producto agregado al catalogo.\n";
                return;
            } else {
                cout << "Cantidad insuficiente en el inventario.\n";
                return;
            }
        }
    }
    cout << "Codigo no encontrado en el inventario.\n";
    guardarInventario(inventario);
}

void QuitarCatalogo(vector<catalogo_ropa> &catalogo, vector<inventario_ropa> &inventario) {
    string codigo;
    int cantidad;

    cout << "\nIngrese el codigo del producto a quitar del catalogo: ";
    cin >> codigo;
    cout << "Ingrese la cantidad a devolver al inventario ";
    cin >> cantidad;
    for (auto &item : catalogo) {
        if (item.codigo_ropa == codigo) {
            if (item.cantidad >= cantidad) {

                if (item.cantidad == cantidad) {
                    for (auto &item : inventario) {
                             if (item.codigo_ropa == codigo) {
                                 item.cantidad += cantidad;
                        }
                    }
                    for (auto it = catalogo.begin(); it != catalogo.end(); ++it) {
                        if (it->codigo_ropa == codigo) {
                            catalogo.erase(it);
                            
                            cout << "Articulo eliminado exitosamente.\n";
                            return;
                        }
                    }
                        
                }
                else {
                    for (auto &item : inventario) {
                             if (item.codigo_ropa == codigo) {
                                 item.cantidad += cantidad;
                        }
                    }

                    item.cantidad -= cantidad; //borrar
                }
                cout << "Producto quitado exitosamente del catalogo.\n";
                return;
            } else {
                cout << "Cantidad no existente en el catalogo.\n";
                return;
            }
        }
    }
    cout << "Código no encontrado en el inventario.\n";
    guardarInventario(inventario);
}

void VerInventario(vector<inventario_ropa> inventario){
    cout << "--------------------------------------------" << endl;
    cout << "\t Ver Inventario" << endl;
    cout << "--------------------------------------------" << endl;

    if (inventario.empty()) {
        cout << "\n\nEl inventario esta vacio" << endl;
        return;
    }   

    for (auto &inventario_ropa : inventario) {
        cout << "\nCodigo: " << inventario_ropa.codigo_ropa << endl;
        cout << "Categoria: " << inventario_ropa.categoria << endl;
        cout << "Marca: " << inventario_ropa.marca << endl;
        cout << "Talla: " << inventario_ropa.talla << endl;
        cout << "Costo Unitario: " << inventario_ropa.costo_unitario << endl;  
        cout << "Cantidad Disponible: " << inventario_ropa.cantidad << endl;
        cout << "Temporada: " << inventario_ropa.temporada << endl;  
        cout << "--------------------------------------------" << endl;
    }
}

void cargarInventario(vector<inventario_ropa> &inventario) {
    ifstream archivo("inventario.txt");
    if (!archivo) {
        return;
    }

    inventario_ropa item;
    while (archivo >> item.codigo_ropa >> item.categoria >> item.marca >> item.talla >> 
                  item.costo_unitario >> item.cantidad >> item.temporada) {
        inventario.push_back(item);
    }

    archivo.close();
}

void guardarInventario(const vector<inventario_ropa> &inventario){
    ofstream archivo("inventario.txt", ios::out | ios::app);
    if (!archivo) {
        return;
    }
        for (const auto &item : inventario) {
        archivo << item.codigo_ropa << " " << item.categoria << " " 
                << item.marca << " " << item.talla << " " 
                << item.costo_unitario << " " << item.cantidad << " " 
                << item.temporada << "\n";
    }

    archivo.close();
}

void HacerPedido(vector<inventario_ropa> &inventario, vector<compra_inventario> &historial_compras, double &presupuesto) {
    string codigo;
    int cantidad;

    cout << "--------------------------------------------" << endl;
    cout << "\t Hacer Pedido" << endl;
    cout << "--------------------------------------------" << endl;

    cout << "\n\nIngrese el codigo de la ropa a comprar (o nuevo codigo si no existe): ";
    cin >> codigo;

    // Buscar si el artículo ya existe en el inventario
    for (auto &item : inventario) {
        if (item.codigo_ropa == codigo) {
            cout << "El articulo ya existe en el inventario.\n";
            cout << "Ingrese la cantidad que desea comprar: ";
            cin >> cantidad;

            double total_costo = cantidad * item.costo_unitario;
            if (presupuesto >= total_costo) {
                item.cantidad += cantidad; // Actualizar inventario
                presupuesto -= total_costo;

                // Registrar la compra
                historial_compras.push_back({codigo, item.categoria, item.marca, item.talla, 
                                             item.costo_unitario, cantidad, total_costo});

                cout << "Compra realizada exitosamente. Presupuesto restante: $" << presupuesto << "\n";
            } else {
                cout << "Presupuesto insuficiente para esta compra.\n";
            }
            return;
        }
    }

    // Si no existe, crear un nuevo artículo
    inventario_ropa nuevo_item;
    cout << "El articulo no existe. Vamos a crearlo.\n\n";
    nuevo_item.codigo_ropa = codigo;

    cout << "Ingrese la categoria: ";
    cin.ignore();
    getline(cin, nuevo_item.categoria);
    cout << "Ingrese la marca: ";
    getline(cin, nuevo_item.marca);
    cout << "Ingrese la temporada: ";
    getline(cin, nuevo_item.temporada);
    cout << "Ingrese la talla: ";
    getline(cin, nuevo_item.talla);
    cout << "Ingrese el costo unitario: ";
    cin >> nuevo_item.costo_unitario;
    cout << "Ingrese la cantidad a comprar: ";
    cin >> cantidad;
    
    double total_costo = nuevo_item.costo_unitario * cantidad;
    if (presupuesto >= total_costo) {
        nuevo_item.cantidad = cantidad;
        presupuesto -= total_costo;

        // Agregar al inventario
        inventario.push_back(nuevo_item);

        // Registrar la compra
        historial_compras.push_back({codigo, nuevo_item.categoria, nuevo_item.marca, nuevo_item.talla, 
                                     nuevo_item.costo_unitario, cantidad, total_costo});

        cout << endl << "Nuevo articulo agregado al inventario.\n";
        cout << endl <<"Compra realizada exitosamente. Presupuesto restante: $" << presupuesto << "\n";
    } else {
        cout << endl <<"Presupuesto insuficiente para crear este articulo.\n";
    }
    guardarInventario(inventario);
    guardarHistorialCompras(historial_compras);
}

void guardarHistorialCompras(const vector<compra_inventario> &historial_compras) {
    ofstream archivo("compras.txt", ios::app);
    if (!archivo) {
        return;
    }

    for (const auto &compra : historial_compras) {
        archivo << compra.codigo_ropa << " " << compra.categoria << " " 
                << compra.marca << " " << compra.talla << " "
                << compra.costo_unitario << " " << compra.cantidad << " " 
                << compra.total_costo << "\n";
    }

    archivo.close();
}


void VerHistorialCompras() {
    ifstream archivo("compras.txt");
    if (!archivo) {
        cout << "No se pudo abrir el archivo de compras o aun no hay historial registrado.\n";
        return;
    }

    cout << "---------------------------------------" << endl;
    cout << "\tHistorial de Compras" << endl;
    cout << "---------------------------------------" << endl;

    string codigo_ropa, categoria, marca, talla;
    double costo_unitario, total_costo;
    int cantidad;

    while (archivo >> codigo_ropa >> categoria >> marca >> talla >> costo_unitario >> cantidad >> total_costo) {
        cout << "\nCodigo de ropa: " << codigo_ropa << endl;
        cout << "Categoria: " << categoria << endl;
        cout << "Marca: " << marca << endl;
        cout << "Talla: " << talla << endl;
        cout << "Costo unitario: $" << costo_unitario << endl;
        cout << "Cantidad comprada: " << cantidad << endl;
        cout << "Total gastado: $" << total_costo << endl;
        cout << "---------------------------------------\n";
    }

    archivo.close();
}

void GenerarCliente(vector<cliente> &clientes, int cantidad_clientes) {

    const vector <string> tipos = {"Premium", "Fashionista", "Frecuente", "Regular"};
    const vector <double> presupuestos = {500.0, 300.0, 200.0, 100.0};

    for (int i = 0; i < cantidad_clientes; i++) {
        cliente nuevoCliente;
        nuevoCliente.codigo_cliente = i + 1;
        int tipoIndex = rand() % tipos.size();
        nuevoCliente.tipo_cliente = tipos[tipoIndex];
        nuevoCliente.numero_visitas = 0.0;
        nuevoCliente.presupuesto = presupuestos[tipoIndex];
        nuevoCliente.total_gastado = 0.0;

        clientes.push_back(nuevoCliente);
    }
}

void GenerarEvento(evento &nuevoEvento) {
    const vector<string> tiposEventos = {"Descuento Especial", "Escasez de Productos", "Promocion Relampago"};
    const vector<double> impactoVentas = {1.2, 0.8, 1.5}; // Incremento/decremento en ventas
    const vector<double> impactoInventario = {0.9, 0.7, 1.3}; // Cambio en inventario

    if (rand() % 2 == 0) { // 50% de probabilidad de ocurrencia
        int indice = rand() % tiposEventos.size();
        nuevoEvento.tipo = tiposEventos[indice];
        nuevoEvento.duracion = rand() % 3 + 1; // Duración de 1 a 3 días
        nuevoEvento.impacto_ventas = impactoVentas[indice];
        nuevoEvento.impacto_inventario = impactoInventario[indice];
    } else {
        nuevoEvento.tipo = "Sin Evento";
        nuevoEvento.duracion = 0;
        nuevoEvento.impacto_ventas = 1.0;
        nuevoEvento.impacto_inventario = 1.0;
    }
}

void SimularCompras(vector<cliente> &clientes, vector<catalogo_ropa> &catalogo, evento eventoDelDia) {
    for (auto &cliente : clientes) {
        cliente.total_productos = 0;
        if (cliente.presupuesto <= 0) continue; // Cliente sin presupuesto
        
        for (auto &producto : catalogo) {
            // Probabilidad de compra según tipo de cliente
            double probabilidad = 
                                   cliente.tipo_cliente == "Premium" ? 0.9 : // 90%
                                   cliente.tipo_cliente == "Raro" ? 0.8 : // 80%
                                   cliente.tipo_cliente == "Frecuente" ? 0.5 : 0.3; // 30%
            
            if ((rand() % 100) < (probabilidad * 100) && cliente.presupuesto >= producto.precio_unitario) {
                int cantidadCompra = min(rand() % 3 + 1, producto.cantidad); // Compra de 1 a 3 unidades
                if (cantidadCompra > 0) {
                    double totalCompra = cantidadCompra * producto.precio_unitario * eventoDelDia.impacto_ventas;

                    // Actualizar datos del cliente
                    cliente.presupuesto -= totalCompra;
                    cliente.total_gastado += totalCompra;
                    cliente.numero_visitas++;

                    // Actualizar inventario
                    producto.cantidad -= cantidadCompra;
                    producto.cantidad_vendida += cantidadCompra; //para reporte de producto mas popular
                    /*if (producto.cantidad <= 5) {
                        producto_a_abastecer =
                    }
                     if (producto.cantidad == 0) {
                    // Crear una copia del producto a eliminar
                        auto producto_a_eliminar = producto;

                        // Buscar y eliminar el producto del catálogo
                        catalogo.erase(std::remove_if(catalogo.begin(), catalogo.end(),
                                                    [&producto_a_eliminar](const catalogo_ropa& p) {
                                                        return p.codigo_ropa == producto_a_eliminar.codigo_ropa;
                                                    }),
                                    catalogo.end());
                    }*/
                    // Actualizar inventario y cantidad vendida
                 
                    
                    // Mostrar compra
                    cout << "\nCliente " << cliente.codigo_cliente <<" ("<<cliente.tipo_cliente<< ") compro " 
                         << cantidadCompra << " de " << producto.codigo_ropa 
                         << " por " <<"------------ $"<< totalCompra <<".\n";

                    presupuesto += totalCompra;
                    cliente.total_productos += cantidadCompra;
                }
            }
        }
        
    }

}

void verReportes(const vector<cliente> &clientes,vector<catalogo_ropa> &catalogo) {
    double gasto_maximo = 0.0;
    int cliente_con_mayor_gasto = 0;
    int max_compras = 0;

    //Producto mas popular
    // Encontrar el producto más vendido
    int cantidad_maxima_vendida = 0;

    cout << "\nProducto(s) mas populares:" << endl;

    for (const auto& producto : catalogo) {
        if (producto.cantidad_vendida > cantidad_maxima_vendida) {
            cantidad_maxima_vendida = producto.cantidad_vendida;
            cout << "  - " << producto.codigo_ropa << " (" <<cantidad_maxima_vendida << " unidades):" << endl;
        } else if (producto.cantidad_vendida == cantidad_maxima_vendida) {
            cout << "  - " << producto.codigo_ropa << endl;
        }
    }

    
    //Cliente con mas cantidad de compras:
    for (const auto& cliente : clientes) {
        max_compras = std::max(max_compras, cliente.total_productos);
    }

    // Encontrar a los clientes con el máximo número de compras
    cout << "\nClientes que compraron mas productos:" << endl;
    for (const auto& cliente : clientes) {
        if (cliente.total_productos == max_compras) {
            cout << "  - Cliente " << cliente.codigo_cliente <<endl;
        }
    }
    cout<<endl;

    //cliente que mas gasto

    for (cliente cliente : clientes) {
        if (cliente.total_gastado > gasto_maximo) {
            gasto_maximo = cliente.total_gastado;
            cliente_con_mayor_gasto = clientes.size(); // Actualizamos el índice del cliente con mayor gasto
        }
    }
    
    cout << "Cliente con mayor gasto: " << endl;
    cout << "  - Cliente " << clientes[cliente_con_mayor_gasto].codigo_cliente <<endl;

    cout << "\nArticulos a reabastecer:" << endl;
    for (const auto& producto : catalogo) {
        if (producto.cantidad <= 5) {
            cout << "  - " << producto.codigo_ropa << " (" << producto.cantidad << " unidades)\n" << endl;
        }
    }
}

void delay (int segundos) {
    clock_t start_time = clock();
    while (clock() < start_time + segundos * CLOCKS_PER_SEC/1000) {}
}
void mostrarBarraProgreso(int duracion) {
    const int total = 40;
    int intervalo = duracion / total;

    cout << "Progreso: [";
    for (int i = 0; i < total; ++i) {
        cout << "#";
        cout.flush();
        delay(intervalo);
    } 
    cout << "] 100%" << endl;
    
}

void CicloDiario(vector<cliente> &clientes, vector<catalogo_ropa> &catalogo){
    evento eventoDelDia;
    GenerarEvento(eventoDelDia);

    cout << "--------------------------------------------" << endl;
    cout << "Iniciando Dia -" << contador_jornadas + 1 << "-" << endl;

    if (eventoDelDia.tipo != "Sin Evento") {
        cout << "\n\nEvento del dia: " << eventoDelDia.tipo << endl;
        cout << "Impacto en inventario: " << eventoDelDia.impacto_inventario << endl;
        cout << "Impacto en ventas: " << eventoDelDia.impacto_ventas << endl;
    } else {
        cout << "No hay evento del dia\n" << endl;
    }

    mostrarBarraProgreso(3000);
    SimularCompras(clientes, catalogo, eventoDelDia);

    cout << "--------------------------------------------" << endl;
    cout << "\nDia Finalizado..." << endl;


    cout << "\n--Quisiera pasar al final de la jornada?--" << endl;

    cout << "\t  1.- Si    2.- No" << endl;
    cout << "\nOpcion: ";

    int opcion;
    cin >> opcion;

    if (opcion == 1) {
        menuFinJornada();
    }
    else if (opcion == 2) {
        cout << "Finalizando..." << endl;
    }
    else {
        cout << "Opcion no valida" << endl;
    }

}


void imprimirMenuEmpleado(const empleado &empleado_activo){
    cout << "--------------------------------------------------------------" << endl;
    cout << "\n\t   Bienvenido " << empleado_activo.nombre_empleado <<endl;
    cout << "\tLe desea Tendencia Total";
    cout << "\n\t\tMenu" << endl;
    cout << "--------------------------------------------------------------" << endl;    
    cout << "\t  1.- Ver Perfil" << endl;
    cout << "\t  2.- Instrucciones" << endl;
    cout << "\t  3.- Planificacion antes de la jornada" << endl;
    cout << "\t  4.- Ver Historial de Compras" << endl;
    cout << "\t  5.- Iniciar Jornada" << endl;
    cout << "\t  6.- Cerrar Sesion" << endl;
    cout << "--------------------------------------------" << endl;
    cout << "\nOpcion: ";
}
void imprimirMenuSubmenu() {
    cout << "--------------------------------------------" << endl;
    cout << "\tPlanificacion antes de la jornada" << endl;
    cout << "--------------------------------------------" << endl;
    cout << "\t\t\t\tDisponible: $" << presupuesto << endl;
    cout << "\t  1.- Ver catalogo" << endl; //tienda: mercancia a la venta y vista de los clientes
    cout << "\t  2.- Ver inventario" << endl;  // stock de la tienda, unidaes disponibles y guardadas
    cout << "\t  3.- Comprar articulos" << endl;  // comprar y registra ropa del proveedor
    cout << "\t  4.- Colocar articulos" << endl;  // agregar artaculos del inventario al catalogo
    cout << "\t  5.- Mis pedidos" << endl; //??? similar o igual a stock, muestra historial de compras a nuestro proveedor
    cout << "\t  6.- Quitar del catalogo" << endl;
    cout << "\t  7.- Volver al menu principal" << endl;
    cout << "--------------------------------------------" << endl;
    cout << "\n\nOpcion: ";
}

void ImprimirmenuFinJornada(){
    cout << "--------------------------------------------" << endl;
    cout << "\tFin de Jornada" << endl;
    cout << "--------------------------------------------" << endl;
    cout << "\t  1.- Ver Reportes" << endl;
    cout << "\t  2.- Logros del dia" << endl;
    cout << "\t  3.- Comprar articulos" << endl;
    cout << "\t  4.- Mis pedidos" << endl;
    cout << "\t  5.- Siguiente dia" << endl;
    cout << "--------------------------------------------" << endl;
    cout << "\nOpcion: ";
}

void SubmenuEmpleado() {
    int opcion;
    do {
        imprimirMenuSubmenu();
        cin >> opcion;
        switch (opcion) {
            case 1:
                //cout << "Ver catálogo" << endl;
                VerCatalogo(catalogo);
                system("pause");
                break;
            case 2:
                //cout << "Ver inventario" << endl;
                VerInventario(inventario);
                system("pause");
                // Agregar lógica específica aquí
                break;
            case 3:
                //cout << "Comprar artículos" << endl;
                HacerPedido(inventario, historial_compras, presupuesto);
                system("pause");
                // Agregar lógica específica aquí
                break;
            case 4:
                //cout << "Colocar artículos" << endl;
                AgregarCatalogo(catalogo, inventario);
                system("pause");
                // Agregar lógica específica aquí
                break;
            case 5:
                //cout << "Mis pedidos" << endl;
                // Agregar lógica específica aquí
                VerHistorialCompras();
                system("pause");
                break;
            case 6:
                QuitarCatalogo(catalogo, inventario);
                system("pause");
                break;
            case 7:
                cout << "Volviendo al menu principal..." << endl;
                system("pause");
                break;
            default:
                cout << "Opcion no válida." << endl;
                system("pause");
                break;
        }
    } while (opcion != 7);
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
                cout << "\t\tTelefono: " << empleado_activo.telefono << endl;
                cout << "\n" << endl;
                system("pause");
                break;
            case 2:
                cout << "\n\n--------------------------------------------" << endl; 
                cout <<"\n \tInstrucciones\t"<< endl;
                cout << "\n--------------------------------------------" << endl;
                cout << "- Planifica tu dia:  Analiza las tendencias y elige las prendas perfectas para tu tienda."<<endl;
                cout << "- Atiende a tus clientes:  Ayuda a cada cliente a encontrar el estilo perfecto y gana dinero."<<endl;
                cout << "- Mejora tu tienda:  Invierte tus ganancias para ampliar tu inventario y decorar tu tienda."<<endl;
                cout << "- Conquista el mundo de la moda:  Desbloquea nuevos niveles, supera desafios y conviertete "<<endl;
                cout << "en el diseniador mas famoso.\n"<<endl;
                system("pause");
                break;
            case 3:
                //cout << "Planificación antes de la jornada" << endl;
                SubmenuEmpleado(); // Llamada corregida
                system("pause");
                break;
            case 4:
                //cout << "Ver historial de compras" << endl;
                VerHistorialCompras();
                system("pause");
                break;
            case 5:
                //cout << "Iniciar jornada" << endl;
                CicloDiario(clientes, catalogo);
                system("pause");
                break;
            case 6:
                cout << "Cerrando sesion..." << endl;
                contador_jornadas = 0;
                system("pause");
                break;
            default:
                cout << "Opcion no valida." << endl;
                system("pause");
                break;
        }
    } while (opcion != 6);
}
void menuFinJornada(){

    int opcion;
    do {
        ImprimirmenuFinJornada();
        cin >> opcion;
        switch (opcion) {
            case 1:
                //cout << "Reportes del dia" << endl;
                    verReportes(clientes , catalogo);
                system("pause");
                break;
            case 2:
                cout << "\n\n--------------------------------------------" << endl; 
                cout << "Logros y Metas" << endl;
                cout << "--------------------------------------------" << endl; 

                cout << "Vendedor Novato: Realiza tus primeras ventas. (bloqueado)\n";
                cout << "Vendedor Experimentado: Alcanza las 100 ventas en un día. (bloqueado)\n";
                cout << "Millonario: Supera las 10000 unidades monetarias en ventas totales en ventas diarias. (bloqueado)\n";
                cout << "Cliente Fiel: Consigue un cliente que realice 5 compras consecutivas. (bloqueado)\n";
                cout << "Mayorista: Realiza una venta de más de 10 unidades de un mismo producto. (bloqueado)\n";
                cout << "Experto en Categorías: Vende al menos un producto de cada categoría. (bloqueado)\n";
                cout << "Coleccionista: Completa tu catálogo de productos.(bloqueado)\n";
                cout << "Veterano: Completa 100 jornadas de trabajo. (bloqueado)\n";
                cout << "Invencible: No te quedes sin stock durante un mes. (bloqueado)\n";
              
                system("pause");
                break;
            case 3:
                //cout << "Comprar articulos" << endl;
                HacerPedido(inventario, historial_compras, presupuesto);
                system("pause");
                break;
            case 4:
                //cout << "Mis pedidos" << endl;
                VerHistorialCompras();
                system("pause");
                break;
            case 5:
                contador_jornadas++;
                cout << "Jornada -" << contador_jornadas << "- finalizada" << endl;
                cout << "Pasando a la siguiente dia..." << endl;
                
                break;
            default:
                cout << "Opcion no valida." << endl;
                system("pause");
                break;
        }
    } while (opcion != 5);
}

// Función principal
int main() {

    system("color d");
    GenerarCliente (clientes, rand() % 30 + 20);
    cargarEmpleados(registro_empleado);
    cargarInventario(inventario);
    menu_Inicio(registro_empleado);
    return 0;
}
