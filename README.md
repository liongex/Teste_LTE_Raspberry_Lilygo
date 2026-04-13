| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C6 | ESP32-H2 | ESP32-P4 | ESP32-S2 | ESP32-S3 |
| ----------------- |   X   | -------- | -------- | -------- | -------- | -------- | -------- | -------- |

# Teste LTE Raspberry Pi + Lilygo

Este projeto contém scripts e configurações para testar a conectividade LTE (4G/3G) utilizando um módulo **Lilygo** (como a série T-PCIE ou T-SIM) em conjunto com um **Raspberry Pi**.  

O objetivo é validar a comunicação serial via comandos AT, verificar a qualidade do sinal e estabelecer conexão de dados.

---

## 🚀 Funcionalidades
- Verificação de status do modem (Comandos AT).
- Leitura de intensidade de sinal (RSSI).
- Teste de envio de SMS.
- Configuração de interface de rede PPP/WVDial (opcional).

---

## 🛠️ Hardware Necessário
- Raspberry Pi (qualquer modelo com GPIO ou USB).
- Módulo Lilygo LTE (ex: T-SIM7600G-H, T-PCIE SIM7000, etc.).
- Antena LTE (e GPS, se aplicável).
- Cartão SIM com plano de dados ativo.
- Cabos jumper ou adaptador USB.

---

## 🔌 Conexão (Pinagem UART)

Se você estiver conectando o módulo via GPIO do Raspberry Pi, utilize a seguinte pinagem padrão:

| Lilygo Module | Raspberry Pi (GPIO) | Pino Físico |
|---------------|----------------------|-------------|
| TX            | RXD (GPIO 15)        | Pino 10     |
| RX            | TXD (GPIO 14)        | Pino 8      |
| VCC           | 5V / 3.3V            | Pino 2 ou 1 |
| GND           | Ground               | Pino 6      |

> **Nota:** Certifique-se de habilitar a porta serial no Raspberry Pi via:
> ```bash
> sudo raspi-config
> ```
> Vá em *Interfacing Options → Serial → Disable Console / Enable Hardware*.

---

## 📦 Instalação e Requisitos

Atualize o sistema:
```bash
sudo apt update && sudo apt upgrade -y

Instale as dependências de Python:

pip install pyserial

Instale ferramentas de depuração (opcional):

sudo apt install minicom screen -y

📂 Estrutura do Repositório

/scripts: Contém scripts Python para testes automatizados.

/docs: Esquemas de ligação e manuais do módulo Lilygo.

/config: Arquivos de configuração para WVDial ou PPP.

🖥️ Como Usar
1. Teste de Comunicação Serial
Para verificar se o Raspberry Pi reconhece o módulo, rode:

python3 scripts/teste_at.py

Verificar Sinal e Operadora

python3 scripts/check_signal.py

Troubleshooting (Resolução de Problemas)
Módulo não responde: Verifique se os cabos TX/RX estão invertidos.

Sinal baixo: Certifique-se de que a antena LTE está bem conectada e que você não está em um subsolo.

Permissão negada na porta: Adicione seu usuário ao grupo dialout:

sudo usermod -a -G dialout $USER

📝 Licença
Este projeto está sob a licença MIT.

👤 Autor
Criado por liongex.
Se este projeto te ajudou, deixe uma ⭐!

