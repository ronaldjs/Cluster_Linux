
# Guía de Implementación de Clúster MPI en el Sistema Operativo Debian

El desarrollo de clúster es importante para conocer la forma en cómo se ejecutan varios procesos sobre varias maquinas (server, cliente1, cliente 2… clienteN), en otras palabras los procesos se ejecutan de forma paralela y realizan una operación determinada en cada una de las maquinas. El objetivo de la guía es llevar a cabo la simulación y creación de un clúster con dos máquinas (Server y Cliente o Master y Slave respectivamente) en el sistema operativo Debian, así como la ejecución de varios algoritmos. Los siguientes pasos serán los requisitos y la forma para crear el clúster.

### 1. Detalles a considerar antes de la Implementación.

El clúster será de tipo Beowulf que es una tecnología para agrupar computadores basados en el sistema operativo Linux para formar un supercomputador virtual paralelo. Algunos de los componentes de hardware y software que se necesitan para el clúster Beowulf se explican posteriormente.

#### 1.1 Requerimientos de Hardware

Beowulf posee una arquitectura basada en multicomputadores el cual puede ser utilizado para computación paralela. Este sistema consiste de un nodo maestro y uno o más nodos esclavos conectados a través de una red Ethernet u otra topología de red. Esta construido con componentes de hardware comunes en el mercado, similar a cualquier PC capaz de ejecutar Linux, adaptadores de Ethernet y switches estándares. Básicamente los requerimientos de hardware que utilizaremos serán los siguientes:

* Una computadora portátil o de escritorio.

* Procesador de 2 GHz de velocidad para soportar las dos máquinas virtuales con el sistema operativo Debian 7 wheezy, en su versión actual.

* Al menos 4 GB de memoria RAM ya que se utilizará como mínimo 1 GB de RAM para cada máquina virtual.

* Contar con una interfaz de red funcional (Cableada o inalámbrica) que debe ser del mismo tipo en todos los nodos.

* Conexión a internet para descargar los paquetes y demás utilidades durante la configuración.

Nota: En el caso de conexión de red o conexión con varios computadores con el objetivo de brindar servicios, será necesario buscar la información pertinente, ya que el clúster se desarrollará en un ambiente virtual con red interna.


### 1.2 Requerimientos de Software

El software que se necesitará para crear el clúster en la distribución de Debian 7, es la siguiente:

* Virtual box en su versión 4.2.6 o superior. EL sistema operativo como anfitrión puede ser cualquiera, debido a que éste no tiene ninguna influencia.
* Tener instalado Debian 7 en su versión más reciente en las dos máquinas virtuales.Ambas maquinas deben tener una cuenta de usuario con el mismo nombre.
* Tener acceso a la Terminal como supe usuario (root).
* Conexión a internet

Nota: Para el clúster es necesario instalar los programas de SSH, NFS, SSH, MPICH2 y el compilador de C++ para Debian. Veremos la instalación de cada uno más adelante en sus correspondientes apartados.

### 1.3 Configuración de la red.

Como el clúster se realizará en dos máquinas virtuales debemos configurar una red interna para la conexión compartida entre ambas máquinas, además se debe usar IPs estáticas en ambas máquinas como un puente de comunicación del clúster.

### 1.3.1 Configuración de la red interna.

La red interna considera como punto vital los siguientes pasos para cada máquina.

a. Seleccionamos nuestra máquina virtual (en nuestro caso Debian 7), luego damos click en configuración y seleccionamos el apartado de red.

![Alt text](https://github.com/ronaldjs/cluster_linux/blob/master/images/imagen1.png?raw=true)


b. Habilitamos el adaptador de red, y lo colocamos conectado a “Red Interna”, dejamos el nombre que viene por defecto y en modo promiscuo seleccionamos “Permitir todo”, las demás configuraciones quedan por defecto y finalmente damos aceptar para guardar los cambios.

![Alt text](https://github.com/ronaldjs/cluster_linux/blob/master/images/imagen2.png?raw=true)

### 1.3.2 Configuración de las IPs estáticas.

La configuración se realiza una vez que estemos adentro del sistema y está dada por los siguientes pasos.

a. Dar click derecho en la parte superior de la pantalla donde se encuentra el icono de conexión de redes y elegir “editar las conexiones” y aparecerá la siguiente ventana. Por defecto aparecerá la pestaña de Cableado, y es precisamente esa en la que se hará la respectiva configuración, independientemente si se usa una red cableada o inalámbrica.

![Alt text](https://github.com/ronaldjs/cluster_linux/blob/master/images/imagen3.png?raw=true)

b. Seleccionar “Wired connection 1” y dar click en el botón “Editar”. Después cambiamos ese nombre por “cluster” u otro, luego elegir la pestaña con el nombre de “Ajuste de IPv4” y seleccionar el método “Manual”.

![Alt text](https://github.com/ronaldjs/cluster_linux/blob/master/images/imagen4.png?raw=true)

c. Para terminar necesitamos escribir la dirección IP estática, en nuestro caso será 192.168.1.1 y 192.168.1.2 para la maquina Master y Slave respectivamente. Se usará un mascara de 24 bits y la puerta de enlace se dejará vacía, normalmente ésta queda con un valor de 0.0.0.0. Después de guardar nos debe quedar de la siguiente forma:

![Alt text](https://github.com/ronaldjs/cluster_linux/blob/master/images/imagen5.png?raw=true)

### 1.3.3 Configuración para internet.

Cuando se finaliza la configuración de las IPs estáticas, la conexión de internet será nula, debido a que modificamos la única conexión de red a internet que venía por defecto. Para configurar una nueva, debemos seguir los mismos pasos para la configuración de red interna, excepto que se debe habilitar otro adaptador (adaptador 2) y conectarlo como
“Adaptador puente”; en el sistema debemos agregar otra conexión cableada, le colocamos un nombre y soló con eso el sistema reconoce automáticamente el adaptador y establece la conexión. Así tenemos conexión compartida entre las máquinas virtuales y acceso a internet.

Nota: Realizar todo estas configuraciones para todos los nodos slaves (maquinas clientes) que conforman el master (maquina servidor).

### 2. Configuración interna de las IPs.

Antes de implementar y efectuar todas las instalaciones necesarias para el clúster, debemos llevar a cabo configuraciones internas de las IPs estáticas.

a. En todas las maquinas se debe modificar el archivo /etc/hosts para colocar cada nombre de cada nodo que participa en el clúster y su respectiva IP. El comando de ejecución sería:

```
root@debian~$ nano /etc/hosts
```

El archivo modificado quedaría así:

![Alt text](https://github.com/ronaldjs/cluster_linux/blob/master/images/imagen6.png?raw=true)

b. En todas las maquinas se debe modificar el archivo /etc/hostname para colocar el nombre de cada nodo que participa en el clúster. El comando de ejecución sería:

```
root@debian~$ nano /etc/hostname
```

El archivo modificado quedaría así:

![Alt text](https://github.com/ronaldjs/cluster_linux/blob/master/images/imagen7.png?raw=true)

### 3. Implementación del Clúster

### 3.1 Instalación de SSH.

Terminadas todas las configuraciones de red, ahora podemos instalar el servidor SSH (Secure
Shell) que es un protocolo usado por excelencia para establecer conexión con máquinas remotas a través de una red. De esta forma se podrá conectar el nodo master con el nodo slave solo con “ssh slave”. Los pasos para instalar y configurar SSH son los siguientes:

a. Instalar openssh-server en el nodos slaves (en nuestro caso será solo uno), si instalamos este paquete durante la instalación de Debian, entonces no será necesario ejecutar el siguiente comando.

```
root@slave~$ apt-get install openssh-server
```

b. Instalar openssh-client en el nodo master, si instalamos este paquete durante la instalación de Debian, entonces no será necesario ejecutar el siguiente comando.

```
root@master~$ apt-get install openssh-client
```

c. Crear una clave RSA en el nodo master

```
root@master~$ ssh-keygen –t rsa –f ~/.ssh/id_rsa
```

d. Activar ssh-agent, lo cual permitirá saber si estamos realizando bien todos los pasos.

```
root@master~$ eval ‘ssh-agent’
…
>> Agent pid 3799 (podría dar otro valor)

```

e. Agregar la clave de autorización con ssh-add

```
root@master~$ ssh-add
```

f. Copiar la clave pública (~/.ssh/id_rsa.pub) a todos los nodos slaves. El comando se ejecutará desde el nodo master y se indicará el nombre de usuario, que en nuestro caso es “debian”.

```
root@master~$scp ~/.ssh/id_rsa.pub debian@slave:~/.ssh/id_rsa.pub
```

g. Agregar el fichero ~/.ssh/authorized_keys para cada nodo slave. Igualmente lo ejecutaremos en el nodo master.

```
root@master~$ cat ~/.ssh/id_rsa.pub &gt;&gt; ~/.ssh/authorized_keys
```

h. Finalmente, para comprobar que el nodo master tiene acceso a los nodos slaves, debemos ejecutar lo siguiente.

```
root@master~$ ssh slave (nombre de los nodos esclavos, puede aplicar IP)
```

Si logramos conectarnos, significa que la maquina master tiene acceso a los nodos slaves y puede manipularlos sin ningún problema.

Nota: Si al momento de intentar ejecutar algunos de los pasos anteriores, arroja un error como “Permiso denegado” o algo por el estilo, intente deshabilitar el firewall, en caso de que el error persista elimine el firewall de la siguiente manera:

```
root@master~$ apt-get remove iptables
```

### 3.2 Instalación del NFS.

NFS (Network File System) permite que un equipo GNU/Linux pueda montar y trabajar con un sistema de archivos de otro equipo de la red como si fuera local. Basicamente, instalaremos NFS para crear una carpeta compartida entre el nodo master y los slaves, en la cual se colocarará los algoritmos para su debida ejecución.

### 3.2.1 Configuración para el servidor (nodo master)

a. Instalar el paquete nfs-kernel- server

```
root@master~$ apt-get install nfs-kernel- server
```

b. Instalar el paquete nfs-common

```
root@master~$ apt-get install nfs-common
```

c. Instalar el paquete portmap

```
root@master~$ apt-get install portmap
```

d. Crear una carpeta, la cual debe nombrarse de la misma forma para cada nodo. La carpeta en nuestro caso será creada en “home” y la nombraremos como “NFS-Compartido”, asi el directorio de la carpeta sería /home/NFS-Compartido. Los pasos son los siguientes:

```
root@master:/home$ mkdir NFS-Compartido

root@master:/home$ chmod 777 NFS-Compartido (Establecer todos los permisos)
```

e. Modificar el archivo de configuración de nfs /etc/exports con nano. Se debe agregar tres campos en la misma línea, el primero corresponde a la nombre del carpeta a compartir, luego la IP de la red (del nodo master) y por ultimos los permisos; a continuación se muestra nuestro caso:

```
/home/NFS-Compartido 192.168.1.1/24 *(rw,sync,no_root_squash)
```

f. Abrir el archivo /etc/hosts.allow con nano y especificar los permisos para la IPs que tendrán acceso a la carpeta. El archivo se le debe agregar:

```
portmap:ALL
nfs:ALL
```

g. Finalmente debemos reiniciar nfs-kernel- server para realizar los respectivos cambios

```
root@master:/home$ service nfs-kernel- server restart
```

ó tambien se puede hacer con: (en caso de que no funcione)

```
root@master:/home$ /etc/init.d/nfs-kernel- server restart
```

### 3.2.2 Configuración para el cliente (nodo slave)

a. Instalar el paquete nfs-common

```
root@slave~$ apt-get install nfs-common
```

b. Instalar el paquete portmap

```
root@slave~$ apt-get install portmap
```

c. Crear una carpeta, que deberá nombrarse igual que la del nodo master y estará ubicada en el mismo directorio especificado en la configuración del master.

```
root@ slave:/home$ mkdir NFS-Compartido
root@ slave:/home$ chmod 777 NFS-Compartido (Establecer todos los permisos)
```

d. Para verificar que todo está bien, podemos verificar con el siguiente comando si la carpeta del servidor está compartida para todos los nodos slaves.

```
root@ slave:showmount –e master (o la IP del nodo master)
>> /home/NFS-Compartido (everyone)
```

e. Luego, se debe establecer la conexión compartida de la carpeta del cliente con la carpeta del servidor. Para eso ejecutamos el siguiente comando.

```
root@ slave:mount –t nfs master:/home/NFS-Compartido /home/NFS-Compartido
```

f. Finalmente se debe editar el archivo fstab con nano, ya que cada vez que reiniciamos las maquinas debemos de ejecutar el paso “e” para que se compartan los archivos de nuevo en la carpeta del nodo slave. Para evitar eso, se le agregará al archivo fstab lo siguiente linea:

```
master:/home/NFS-Compartido /home/ NFS-Compartido nfs
```

### 3.3 Instalación y Configuración del MPICH2

Es recomendable utilizar el administrador de paquetes para buscar la versión más actual de MPICH para debían. Se instala mpich2 en el master y en todos los nodos esclavos.

```
root@master: $ apt-get install mpich2
root @slave: $ apt-get install mpich2
```

### Configurar en el master:

Listar los nodos que participaran en el cluster en el archivo /home/compartido/hosts. Este archivo se ubicara dentro de la carpeta compartida y no es necesaria una extensión, el archivo debe ser creado y se debe ingresar en él mismo, la siguiente información. Se puede ingresar la dirección IP para evitar cualquier confusión con los nombres en los hosts, si no sirviera de una manera probar de la otra. Como el archivo se encontrará en la carpeta compartida que creamos no es necesario definir nada en el esclavo porque este utilizará el mismo archivo.

```
Master
Slave
```

Nota: el compilador por defecto en Debian es c, pero como en este caso se correrán ejemplos de C++ es necesario instalar su compilador, para ello se ejecuta la siguiente línea y ya estará listo para usar:

```
root@master: $ apt-get install build-essential
```

### 3.4 Correr el Código

Todo esto se realizara en el master.

a. Antes de correr el código el mismo debe ubicarse en la carpeta compartida entre nodos master y slaves. Lo primero a realizar es compilar el archivo con el código, con la siguiente instrucción que ejecutará el archivo ejemplo.cpp y lo convertirá a ejecutable.

```
root@master~: $ mpic++ /home/NFS-Compartido/ejemplo.cpp –o /home/NFS-Compartido/ejemplo
```

b. Luego, se corre el comando para poder ejecutar el ejecutable. Se indicará el nombre del archivo con los nodos correspondientes al cluster y se indicará la cantidad de procesos que se quieren correr en este caso un 10.

```
root@master~: $ mpiexec –f /home/NFS-Compartido/hosts -n 10 /home/NFS-Compartido/ejemplo
```

# Algoritmos Implementados

### a. Descripción

### a.1. Mergesort

El algoritmo ordena un vector con números desordenados, el proceso cero (nodo master) toma el vector y el número de procesos que se van a ejecutar y reparte el vector de forma equitativa a los todos los procesos (incluyéndose). Cada proceso ordena de manera local la lista asignada, la mezcla (merge) se hace uniendo dos procesos e igual se efectúa la ordenación y así sucesivamente hasta llegar a ordenar todo el vector. Cada vez que se realiza la mescla y se ordena, un proceso dejará de trabajar de cada grupo, así por ejemplo si hay tres grupos trabajando paralelamente en la primera mezcla (es decir, 6 procesos en total), luego quedarán por fuera 3 procesos y así se hará de nuevo otra mezcla, pero como la cantidad es impar, un proceso quedará en espera por el nuevo resultado, este caso es el único que no se hace paralelo, pero en la ordenación los procesos se ejecutan de forma paralela.

### a.2. Multiplicación de Matrices

El algoritmo toma dos matrices, una matriz cuadrada (NxN) y una matriz columna (Nx1). El proceso cero (nodo master) tiene al principio las dos matrices y las llena con número aleatorios, luego reparte la filas de la matriz cuadrada a la cantidad de procesos que se van a ejecutar (el número de procesos debe ser igual a la dimensión de la matriz cuadrada). Cuando se hace la distribución, los procesos hacen las operaciones de la fila por la matriz columna y al final manda el resultado a proceso cero, quien muestra el resultado final.

### a.3. Factorización de números primos

El algoritmo realiza la ejecución paralela únicamente con uno o dos procesos, si es un proceso, entonces lo ejecuta de manera secuencial, en caso contrario, los primos del número a factorizar son obtenidos de manera paralela, donde cada proceso (de los dos que existen) sincroniza sus resultados para encontrar los números primos. La operación se realiza a través de una variable compartida, que contendrá el número y sus factores equivalentes.

### b. Prueba de Ejecución de los Algoritmos

### b.1. Mergesort

a. Compilación correcta del Código

![Alt text](https://github.com/ronaldjs/cluster_linux/blob/master/images/imagen8.png?raw=true)

b. Ejecución del algoritmo y resultados

![Alt text](https://github.com/ronaldjs/cluster_linux/blob/master/images/imagen9.png?raw=true)

### b.2. Multiplicación de Matrices

a. Compilación correcta del Código

![Alt text](https://github.com/ronaldjs/cluster_linux/blob/master/images/imagen10.png?raw=true)

b. Ejecución del algoritmo y resultados

![Alt text](https://github.com/ronaldjs/cluster_linux/blob/master/images/imagen11.png?raw=true)

### b.3. Factorización de números primos

Ejecución del algoritmo y resultados

![Alt text](https://github.com/ronaldjs/cluster_linux/blob/master/images/imagen12.png?raw=true)

