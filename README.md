# USB 2.0 HID stack on stm32f407
##  overview

### Spec :
https://www.usb.org/sites/default/files/usb_20_20190524.zip
[USB PROTOCOLs](https://beyondlogic.org/usbnutshell/usb1.shtml)

### Dev Kit : 

[stm32f407_discovery](https://www.st.com/content/ccc/resource/technical/document/user_manual/70/fe/4a/3f/e7/e1/4f/7d/DM00039084.pdf/files/DM00039084.pdf/jcr:content/translations/en.DM00039084.pdf)
[stm32f407VG](https://www.st.com/content/st_com/en/products/microcontrollers-microprocessors/stm32-32-bit-arm-cortex-mcus/stm32-high-performance-mcus/stm32f4-series/stm32f407-417/stm32f407vg.html)

### Toolchain :
[Gnu Arm embedded toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
[Zadig USB DRIVE](https://zadig.akeo.ie/)
[STlink Driver](https://www.st.com/en/development-tools/stsw-link009.html)

### Programer and Debugger :
[OpenOCD Tutorial](https://ithelp.ithome.com.tw/articles/10192744)
[OpenOCD 4 Windows](https://gnutoolchains.com/arm-eabi/openocd/)

### Repo : 

[GitURL](https://github.com/a910184/JFF.git)

### before main() :

[build code reference]
(https://github.com/jserv/mini-arm-os)
底層的C語言世界
https://ccckmit.gitbooks.io/lowlevelc/content/io.html
### uart_debug (eznob ongoing)


## Openocd / Telnet / GDB
### Openocd
載入寫好之cfg File
```
openocd -f ./xxx.cfg
```
![cfg file](https://i.imgur.com/8K05Cfa.jpg)

載入完成後會告知當前晶片型號、Debug接口類型與GDB/Telnet Port Number

![openocd inform.](https://i.imgur.com/vSfY5C8.jpg)
### Telnet
接著必須維持此CMD開啟，並開一新CMD連上TelnetServer(Port 4444)
之後可輸入help、targets等指令觀察晶片狀態，輸入exit即結束
```
telnet localhost 4444
```
成功連上會顯示Open On-Chip Debugger
![telnet server](https://i.imgur.com/ZHNpbdG.jpg)
```
targets
```
顯示當前狀態
![targets](https://i.imgur.com/Vde4t6n.jpg)
### GDB
輸入arm-none-eabi-gdb會顯示版本號等資訊
```
arm-none-eabi-gdb
```
![GDB infrom.](https://i.imgur.com/3UPpkBW.jpg)
接著連上GDB Srver、成功會顯示當前記憶體位置
```
target remote :3333
```
![gdb server](https://i.imgur.com/O6EiErU.jpg)
欲使用Openocd指令須加上monitor關鍵字
```
monitor targets
```
![monitor targets](https://i.imgur.com/ZWEEcvz.jpg)
之後可開始載入檔案(elf、bin)
gdb 從elf獲取當前燒錄程式的資訊，得知中斷點的Program Counter位置或是中斷條件。並控制CPU停在軟體中斷點位置。存取特定記憶體位置的資料。
```
file ./xxx.elf
monitor reset 
monitor halt
monitor load (下載程式至Flash)
```

## USB OTG REGISTER
外設模式支援IN OUT(EP0+EP1~3)共8通道，並自帶FIFO
![模式介紹](https://i.imgur.com/1d1ccZ9.jpg)
![ENDPOINT介紹](https://i.imgur.com/rBNCdks.jpg)
![ENDPOINT介紹](https://i.imgur.com/ibg8wyA.jpg)
![ENDPOINT控制](https://i.imgur.com/YxnGkod.jpg)
![ENDPOINT傳輸](https://i.imgur.com/mygdYTv.jpg)
![ENDPOINT中斷](https://i.imgur.com/DsmIXdz.jpg)



### CSR MEMORY MAP
![MEMORY MAP](https://i.imgur.com/cgz4QIV.jpg)
### DEVICE MODE MEMORY MAP
OTG_FX_AAAAx x<-表示EP號碼，Address Offset為AddressBase + 0x20*x，x=1~3
![DEVICE MODE MEMORY MAP1](https://i.imgur.com/hC6GfFd.jpg)
![DEVICE MODE MEMORY MAP2](https://i.imgur.com/wutDHdR.jpg)

### DATA FIFO ACCESS REGISTER MAP
![DATA FIFO ACCESS MEMORY MAP](https://i.imgur.com/ozm5Rsy.jpg)

### CLOCK GATE CSR MAP
![CLOCKGATE](https://i.imgur.com/mzkntX3.jpg)

### DEVICE INIT. STEPS
![INIT STEP](https://i.imgur.com/Rsihpr1.jpg)




FIFO REG   待完成 (在RCC_AHB REGISTER下)

[-----REF----](https://www.crifan.com/files/doc/docbook/usb_basic/release/pdf/usb_basic.pdf)



# USBD ENUM INITIAL STEPS

1. Setup Transcation (OUT)
- Token
![](https://i.imgur.com/2CfZGa3.png)
- Data
![](https://i.imgur.com/juOkuah.png)
- HANDSHAKE
![](https://i.imgur.com/kw1NDCp.png)

2. GET DESCRIPTOR (IN)
![](https://i.imgur.com/xQKHmUa.png)
↑NAK means there are no data to be sent,and no address / endpoint cauz of init condition.
The HOST will poll and poll in token to DEVICE until it answers the call.
If not , the DEVICE will be recongnized as offline.
![](https://i.imgur.com/sBellyU.png)

3. SET ADDRESS (OUT)
![](https://i.imgur.com/Ktl0x3L.png)

4. GET CONFIGURATION (IN) 
![](https://i.imgur.com/imL3pLE.png)


5. GET INTERFACE (IN)
![](https://i.imgur.com/8qKq78s.png)
![](https://i.imgur.com/E31RlPy.png)
![](https://i.imgur.com/XteXgUA.png)

6. GET ENDPOINT (IN)