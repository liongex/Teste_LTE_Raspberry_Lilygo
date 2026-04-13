| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C6 | ESP32-H2 | ESP32-P4 | ESP32-S2 | ESP32-S3 |
| ----------------- |   X   | -------- | -------- | -------- | -------- | -------- | -------- | -------- |

# LilyGo T-SIM A7670X - Serial Bridge (Firmware ESP32)

Este repositório contém o firmware desenvolvido em **ESP-IDF** para as placas **LilyGo** equipadas com o modem **A7670X**.  
O código transforma o **ESP32** em uma ponte de comunicação entre a porta USB (Serial 0) e o modem LTE (Serial 1).

---

## 📝 Descrição do Projeto
Ao carregar este código no ESP32, ele assume as seguintes responsabilidades:

- **Gerenciamento de Energia:** Ativa o chip IP5306 via I2C para fornecer energia ao sistema.  
- **Boot do Modem:** Realiza a sequência de pulsos no pino PWRKEY para ligar o modem automaticamente.  
- **Ponte Transparente:** Utiliza tarefas em paralelo (FreeRTOS) para repassar cada byte recebido na USB para o modem e vice-versa, permitindo o uso de comandos AT diretamente de um terminal externo.  

---

## 🛠️ Especificações de Hardware (Configuradas no Código)

O firmware utiliza a seguinte configuração de pinagem interna da placa LilyGo:

| Recurso       | Pino ESP32  | Descrição                          |
|---------------|-------------|------------------------------------|
| Modem TX      | 26          | Transmissão para o Modem           |
| Modem RX      | 25          | Recepção do Modem                  |
| Modem PWR_PIN | 4           | Chave de ignição (PWRKEY)          |
| I2C SDA       | 21          | Dados para gerenciador IP5306      |
| I2C SCL       | 22          | Clock para gerenciador IP5306      |
| USB UART      | UART_NUM_0  | Comunicação com o PC/Raspberry     |

---

## 🚀 Fluxo de Funcionamento

O código opera através de duas tarefas principais executadas em núcleos/tempos paralelos:

- **usb_to_modem_task:** Monitora a porta USB e encaminha os dados para o modem.  
- **modem_to_usb_task:** Monitora a resposta do modem e encaminha para a porta USB.  

---

## 🔧 Como Compilar e Gravar

Este projeto foi desenvolvido utilizando o **ESP-IDF**.

Configure o ambiente:
```bash
. $HOME/esp/esp-idf/export.sh

Compile o projeto:

bash
idf.py build
Grave no ESP32:

bash
idf.py -p [PORTA_COM] flash monitor

---

## 🖥️ Como Testar

Após gravar o firmware, o ESP32 iniciará o modem automaticamente (você verá os logs Iniciando hardware do modem...).

Abra um terminal serial (VS Code Serial Monitor, Putty ou screen) na porta USB do ESP32.

Configure a velocidade para 115200 baud.

Digite comandos AT. Exemplos:

AT → Resposta esperada: OK

ATI → Resposta esperada: Informações do modelo do modem

AT+CSQ → Resposta esperada: Qualidade do sinal

---

## ⚠️ Observações Importantes
Alimentação: O modem LTE consome picos de corrente elevados durante a busca de rede. Use um cabo USB de boa qualidade ou uma bateria conectada à placa.

Sequência de Boot: O código aguarda 5 segundos após ligar o modem antes de liberar a ponte serial, garantindo que o modem esteja pronto.

---
## 👤 Autor
Projeto adaptado para LilyGo T-SIM Series por liongex.



