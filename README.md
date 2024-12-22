# TP de Synthèse – Autoradio
Ce TP propose d'utiliser une NUCLEO_L476RG afin de rediriger un signal audio. Nous utiliserons un shield afin de pouvoir interfacer la board avec les différents inputs et outputs nécessaires.
 
Voir ici la [datasheet : audio interface](./audio_iface.pdf)

Nous avons mis un point d'honneur sur l'organisation des fichiers. Pour vous aidez à vous y retrouver, nous avons documenté avec Doxygen le projet.

>[Vue d'ensemble DOXYGEN](https://romingodelfuego.github.io/2425_ESE_AutoRadio_CARONELLO_PACE/files.html)
#### Milestone
- [x] [1. Shell RTOS](#1-démarrage)
- [x] [2. Driver LED](#23-driver)
- [x] [3.4 Générer un signal audio](#34-génération-de-signal-audio)
- [x] [3.5 Rediriger un signal audio](#35-bypass-numérique)
- [ ] 4. Visualiser au Vu mettre
- [ ] 5. Filtre RC
- [ ] 6. Effet Audio

## 1. Démarrage
On souhaite ici implémenter un shell dans un os temps réel. On créer donc une tache avec une gestion d'erreurs
```` c
xTaskCreate(shell_run,	"Shell", TASK_STACK_DEPTH_SHELL, NULL, TASK_PRIORITY_SHELL, &h_task_shell) != pdPASS ? Error_Handler():(void)0;
````

## 2. Le GPIO Expander et le VU-Metre
### 2.1 Configuration
1. Quelle est la référence du GPIO Expander ? Vous aurez besoin de sa datasheet, téléchargez-la.
    > [MCP23S17-E/SO](./MCP23017_Data_Sheet_DS20001952-2998473.pdf)
2. Sur le STM32, quel SPI est utilisé ?
    >On utilise le SPI3 avec le branchement suivant 
    >````
    >PB5    | SPI3_MOSI 
    >PC10   | SPI3_SCK
    >PC11   | SPI3_MISO 
    >````
3. Quels sont les paramètres à configurer dans STM32CubeIDE ?
    > Frame Format : MOTOROLA \
    > Data Size : 8 bits \
    > First Bit : MSB
    > On réduit également le Baud rate à 312.5 kBits/s
### 2.3 Driver
On écrit un driver qui communique avec le GPIO expander pour allumer ou eteindre les LEDs qui y sont reliée à ces branches.
````h
void MCP23S17_init(void);
HAL_StatusTypeDef MCP23S17_WriteRegister(uint8_t reg, uint8_t data);
uint8_t MCP23S17_ReadRegister(uint8_t reg);
````
Dans le shell on utilise ces fonctions pour choisir l'état de la LED.
```` c
void subfunct_led(char **argv){
	int GPIO = atoi(argv[1]);
	uint8_t i=(uint8_t) strtol(argv[2], NULL, 10);
	if (GPIO == atoi("A")){
		MCP23S17_WriteRegister(0x12, (0xFF<<i)&0xFF) == HAL_OK ? debug(START,"MCP23S17 - GPIOA") : debug(D_ERROR,"MCP23S17 - GPIOA");
	}else if (GPIO == atoi("B")){
		MCP23S17_WriteRegister(0x13, (0xFF<<i)&0xFF) == HAL_OK ? debug(START,"MCP23S17 - GPIOB") : debug(D_ERROR,"MCP23S17 - GPIOB");
	}else {
		debug(D_ERROR,"Argument 2 : A ou B attendu\r\n");
	}
}
````
## 3 Le CODEC Audio SGTL5000
### 3.1 Configuration préalables
1. Quelles pins sont utilisées pour l’I2C ? À quel I2C cela correspond dans le STM32 ?
    > On utilise le I2C2 avec le branchement suivant :
    >````
    >PB10   | I2C2_SCL 
    >PB11   | I2C2_SDA
    >````
### 3.2 Configuration du CODEC par l’I2C
On écrit également un driver pour communiquer avec le codec 
````h
int sgtl5000_i2c_read_register(uint16_t reg_address, uint16_t * p_data);
int sgtl5000_i2c_write_register(uint16_t reg_address, uint16_t data);
int sgtl5000_i2c_set_bit(uint16_t reg_address, uint16_t mask);
int sgtl5000_i2c_clear_bit(uint16_t reg_address, uint16_t mask);
````
### 3.4 Génération de signal audio
Dans le .ioc, on configue le block A2 pour être en `Master with Master Clock` pour gérer la transmission. Et le block B2 comme un `Synchronous Slave` pour gérer la réception.
Pour générer un signal triangle, on n'active seulement le canal A2 du I2S et on transmet  par DMA le bufffer rempli pour être un triangle.

````c
for (int i = 0; i < SIZE_SAI_BUFFER / 2; i++) {
    // Montée du triangle
    data_SAI_rx[i] = (uint16_t)((float)i / (SIZE_SAI_BUFFER / 2) * UINT16_MAX);
}
for (int i = SIZE_SAI_BUFFER / 2; i < SIZE_SAI_BUFFER; i++) {
    // Descente du triangle
    data_SAI_rx[i] = (uint16_t)((float)(SIZE_SAI_BUFFER - i) / (SIZE_SAI_BUFFER / 2) * UINT16_MAX);
}
__HAL_SAI_ENABLE(&hsai_BlockA2);
HAL_SAI_Transmit_DMA(&hsai_BlockA2, (uint8_t*)data_SAI_rx, SIZE_SAI_BUFFER);
````

### 3.5 Bypass Numérique
Pour créer un bypass il nous suffit de recevoir le signal depuis le block B2 pour ensuite le transmettre sur le block A2. Ici nous ne faisons aucun traitement sur le signal.
````c
__HAL_SAI_ENABLE(&hsai_BlockB2);
__HAL_SAI_ENABLE(&hsai_BlockA2);
HAL_SAI_Receive_DMA(&hsai_BlockB2, (uint8_t*)data_SAI_rx, SIZE_SAI_BUFFER);
HAL_SAI_Transmit_DMA(&hsai_BlockA2, (uint8_t*)data_SAI_rx, SIZE_SAI_BUFFER)
````
Nous avons tester en envoyant depuis un port jack de nos ordinateurs une musique, puis nous avons ecouter depuis la prise jack de sortie sur des ecouteurs filaires la musique.
Le son n'est vraiment pas bon, il semble que les aigues soient complètement filtrés. 

### 4. Visualisation
Nous êtions proches de proposer un vu-metre sur ce TP.
Nous avions créer une tache, créer des notifications lors de la fin de la réception DMA qui notifiait la tache VU-Metre. Le point bloquant que nous n'avons pas eu le temps de traiter est l'allumage des LED selon la specification VU.

Dans le code, nous avons tenter une première méthode dans laquelle nous regardions si la moyenne du signal était comprise entre certain seuil. Malheureusement ce n'est pas ainsi que l'echelle VU fonctionne.

Il fautdrait donc implémenter une correspondance entre le signal et l'échelle VU : [WIKIPEDIA - Vu Mètre](https://fr.wikipedia.org/wiki/VU-m%C3%A8tre)


## Doxygen en GithubPages 
### Installation de Doxygen
- Installer doxyfile
    - MacOs : 
    ````
    brew install doxygen
    ````
    - Linux : 
    ```
    sudo apt install doxygen
    ```
- Créer un doxyfile dans votre répertoire 
```bash
doxygen -g
open Doxyfile
```
### Configuration du Doxyfile

- Pour que le doxyfile créer un dossier avec tous les documents:
````bash
OUTPUT_DIRECTORY        = ./Architecture
````
- Pour que le doxyfile cherche dans les dossiers:
````bash
RECURSIVE               = YES
````
- Compiler le doxygen 
````bash
doxygen doxyfile
open ./Architecture/html/index.html
```` 

### Publier sur le GitHub Pages
[Références API GITHUB](https://docs.github.com/en/rest/pages/pages?apiVersion=2022-11-28#create-a-github-pages-site)
Pour créer une pages
````r
# GitHub CLI api
# https://cli.github.com/manual/gh_api

gh api \
  --method POST \
  -H "Accept: application/vnd.github+json" \
  -H "X-GitHub-Api-Version: 2022-11-28" \
  /repos/OWNER/REPO/pages \
   -f "source[branch]=main" -f "source[path]=/Architecture/html/"
`````

Pour associer le Doxygen a cette page
````r
gh api \                                                                                                                                      romingo@RoMackBook-Air
  --method POST \
  -H "Accept: application/vnd.github+json" \
  -H "X-GitHub-Api-Version: 2022-11-28" \
  /repos/OWNER/REPO/pages 
  ````