# UNIVERSIDAD AUTÓNOMA DE NUEVO LEÓN

## FACULTAD DE CIENCIAS FÍSICO MATEMÁTICAS

Estructura de Datos

**Instructor:** Ernesto Jesus Solis Valenzuela 

**Actividad:** Propuesta para Producto Integrador de Aprendizaje

**Datos del Equipo:**

* **Número de Equipo:** 1  
* **Grupo:** 033  
* **Integrantes:**  
  * 2222332 \- Luis Daniel Salazar Sanchez  
  * 2225447 \- Ethan Abdiel Espinoza Acosta  
  * 2243962 \- Liam Javier Castillo Pacheco

**Lugar y Fecha:** San Nicolás de los Garza, N.L., a 16 de abril de 2026

**Introducción**

En la fase de expansión de un Proveedor de Servicios de Internet (ISP), el despliegue de nueva fibra óptica desde la Central (OLT) hacia nuevas zonas de usuarios (Splitters) representa un desafío logístico. En entornos urbanos, la infraestructura existente ofrece múltiples rutas posibles como tendidos aéreos en postes de luz, ductos subterráneos y registros para interconectar dos puntos.

El problema fundamental radica en que la ruta física más corta rara vez es la ruta óptima en telecomunicaciones. La degradación de la señal no depende exclusivamente de la longitud del cable, sino que es un fenómeno acumulativo. Mientras que cada segmento de fibra introduce atenuación lineal por kilómetro, cada cambio de dirección, pozo de visita o cruce urbano requiere empalmes de fusión que añaden severas atenuaciones puntuales.

Elegir una ruta priorizando la conveniencia logística sobre la física óptica, frecuentemente resulta en un Presupuesto de Potencia (Power Budget) deficiente. Esto provoca que la señal llegue degradada o inservible al final del trayecto, obligando al ISP a incurrir en costosos rediseños de planta externa. Para mitigar este riesgo operativo, los ISP requieren una herramienta predictiva que evalúe todas las topologías de las calles disponibles y determine el trazado exacto que garantice la menor degradación de señal posible, validando la viabilidad del servicio antes de realizar cualquier inversión en campo.

**Definición PIA**

Se propone el desarrollo de un motor de optimización y auditoría predictiva especializado en el diseño de infraestructuras de fibra óptica. El software funcionará como un sistema de simulación que procesa la topología física urbana incluyendo postes, ductos y registros existentes para identificar el trazado de cableado más eficiente desde el nodo central hasta los puntos de distribución final.

A diferencia de los métodos de planificación convencionales que se limitan a la métrica de distancia geométrica, este sistema integra un modelo de evaluación basado en la **degradación óptica acumulada**. Al procesar variables críticas como la atenuación propia del medio y las pérdidas por inserción en cada punto de interconexión (mufas o registros), la herramienta permite al ISP realizar una validación técnica rigurosa del presupuesto de potencia en la fase de diseño.

De esta manera, el personal de ingeniería podrá simular escenarios de despliegue y validar matemáticamente que la ruta seleccionada garantizará la integridad de la señal antes de desplazar cuadrillas al campo. Esto no solo asegura que el diseño final sea operacionalmente viable, sino que elimina la incertidumbre técnica, reduce drásticamente la necesidad de rediseños correctivos post-instalación y optimiza la inversión en materiales y capital humano.

**Propósito General**

Proveer a las áreas de ingeniería de los ISP una herramienta algorítmica automatizada que planifique despliegues físicos de fibra óptica, garantizando la viabilidad del servicio mediante la selección de rutas de mínima atenuación de señal.

# 

# **Propósitos Específicos**

* Modelar digitalmente la infraestructura urbana de telecomunicaciones (postes de paso, pozos de visita, mufas de empalme).  
* Automatizar el cálculo de la atenuación óptica acumulada considerando el coeficiente de degradación lineal de la fibra y las pérdidas puntuales por elementos pasivos.  
* Determinar el trazado físico que presente la menor resistencia óptica entre una central transmisora y un punto de distribución final.  
* Emitir un reporte de viabilidad indicando si la potencia final calculada cumple con los umbrales de sensibilidad de los equipos receptores.

**Propuesta de Solución**

Para resolver la multiplicidad de caminos físicos y calcular la ruta óptima, la infraestructura urbana se modelará como un **Grafo Ponderado**. Los vértices (nodos) representarán los puntos geográficos de interconexión, categorizados por su tipo físico (postes, registros con empalme, gabinetes). Las aristas (enlaces) representarán los trayectos de tendido disponibles.

El aspecto central de la solución radica en la función de peso de las aristas. El peso no será la longitud del cable entre dos vértices, sino el **Costo de Atenuación Óptica** expresado en decibelios (dB). El sistema evaluará el grafo mediante algoritmos de búsqueda de caminos mínimos para identificar la sucesión de vértices que acumule la menor cantidad de dB, asegurando así la máxima preservación de la potencia del láser.

![][image1]

**Figura 1\.** Topología de malla irregular para la optimización de rutas ópticas en entorno urbano (21 nodos).

**Nomenclatura de Vértices (Nodos):**  
Representados bajo la sintaxis **(Tipo)ID**, donde el color identifica la atenuación intrínseca del elemento pasivo:

* \[Rojo / Cuadrado\] (O): Central Office (OLT). Punto de origen de la señal.  
* \[Gris\] (P): Poste de paso. Atenuación de 0.0 dB.  
* \[Naranja\] (E): Mufa de empalme. Penalización de 0.1 dB por fusión de fibra.  
* \[Azul\] (G): Gabinete o punto conectorizado. Penalización de 0.5 dB por transición mecánica.  
* \[Verde / Doble Círculo\] (S): Splitter de distribución. Nodo destino para el cálculo de ruta.

**Nomenclatura de Aristas (Enlaces):**  
Representadas bajo la sintaxis **{Tipo}{Distancia, Coeficiente, Pérdida}**:

* Tipo (A/U): Identifica si el despliegue es Aerial (Aéreo) o Underground (Subterráneo).  
* Valores: {km, dB/km, dB\_totales}. Ejemplo: {0.5, 0.35, 0.17} indica un tramo de 500 metros con una pérdida de 0.17 dB.

**Proceso Operativo**

**Costo estimado:** 60,000.00 MXN

**Equipos:** 4

**Programadores:** 3

**Sueldo:** 20,000.00 MXN por programador

**Diseño**

El software se construirá en lenguaje C, se utilizara una **Lista de Adyacencia** para representar el grafo disperso.

Las estructuras de datos base serán las siguientes:

![][image2]

**Implementacion Algorítmica**

Para el calculo del trazado de la ruta se hara una adaptación del **Algoritmo de Dijkstra**. La diferencia principal de esta implementación respecto a su uso tradicional radica en su función de costo: en lugar de buscar la ruta geográficamente más corta, el algoritmo buscara el trayecto de menor resistencia óptica.

Para lograr esto, la fase de exploración no se limitará a sumar el peso estático de las aristas. En cada visita a un nodo adyacente, el sistema calculará dinámicamente la **pérdida de potencia óptica acumulada** sumando dos factores físicos críticos:

1. **La atenuación del enlace:** El costo proporcional a la distancia del segmento de fibra (peso de la arista).  
2. **La atenuación intrínseca:** La penalización óptica generada por el tipo de infraestructura pasiva en el punto de interconexión (ej. 0.0 dB para paso aéreo, 0.1 dB para empalmes de fusión o 0.5 dB para conectores).

De esta manera, el algoritmo evalúa el costo real del Presupuesto de Potencia en cada iteración, permitiendo que el sistema descarte rutas físicamente cortas pero con excesivos empalmes, en favor de rutas ligeramente más largas pero ópticamente más limpias. 

**Flujo Operativo**

El simulador implementará una interfaz de línea de comandos estructurada en fases lógicas, permitiendo una gestión integral del ciclo de vida del diseño de red:

1. **Gestión de Infraestructura (Carga y Persistencia):**  
   * **Creación y Edición:** El usuario podrá registrar manualmente nuevos elementos de infraestructura (postes, registros o mufas) y establecer los vínculos físicos entre ellos directamente desde la interfaz.  
   * **Persistencia de Datos:** El sistema permitirá guardar la topología diseñada en archivos externos ( .csv) y cargarlos posteriormente, facilitando el intercambio de escenarios de red entre diferentes equipos de ingeniería.  
   * **Construcción Dinámica:** Al cargar o crear un mapa, el software instanciará el grafo en memoria, calculando automáticamente el peso óptico inicial de cada enlace basándose en los parámetros físicos definidos.  
2. **Ejecución de Optimización de Ruta:**  
   * Partiendo de un Nodo Central (OLT) definido, el usuario seleccionará el punto de entrega deseado (Nuevo Splitter).  
   * El motor algorítmico ejecutará la **optimización de ruta**, evaluando sistemáticamente todos los caminos alternativos y acumulando las pérdidas por distancia y por hardware pasivo hasta determinar el trayecto de mínima atenuación.  
3. **Reporte de Ingeniería y Viabilidad:**  
   * **Trazado Físico:** El sistema realizará un rastreo inverso para imprimir la secuencia física exacta del despliegue.  
   * **Análisis de Señal:** Se presentará la atenuación total esperada en decibelios (dB).  
   * **Reporte de Viabilidad:** El software validará si la potencia resultante se mantiene por encima del umbral de sensibilidad estándar (-28 dBm). En caso de que la señal sea inferior, el sistema emitirá una alerta de "Señal Crítica", indicando que la ruta, aunque sea la mejor disponible, requiere el uso de amplificadores o un rediseño del trayecto.

