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