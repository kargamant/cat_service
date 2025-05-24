# cat_service
This is a simple web-service for feeding a cat... Written with C++ using winsock2 library.  

# Basic concept  

There is a special cat on a server side that client can communicate with.  
Client can feed or pet the cat and get response.  
The system is following.  
![cat_service drawio](https://github.com/user-attachments/assets/e1e18a75-7ee1-4ed9-8c9d-6212142aa97f)  

# Feed the cat service  

The cat is a bit crazy, so it accepts only alchocol and ciggarettes and rejects other food...  
Requests are made via UDP in following format: 
```
@<nickname> - <food>~
```
If food is accepted cat sends Eaten response, if not - ignored response.  
  
Here is an example of basic communication:  
![console_dialogue](https://github.com/user-attachments/assets/16d42f85-7874-4702-aa9a-9fdec75dc2fb)  
  
And the traffic is following:  
  
![pos_request](https://github.com/user-attachments/assets/c8d02cea-da5e-4094-bf90-6ccbb46bdfc5)  
![pos_response](https://github.com/user-attachments/assets/bbd13e7c-f011-4b5f-bff7-e679e40825ac)  
full dump [here](https://disk.yandex.ru/d/GVCHHr0cq9z_Rw)  

Also fragmentation was made.  So if a client have a very long nickname that does not fit in buff_size, then several UDP fragments will be sent.  
The format of fragment is following:  
```
<fragment_data>~<number_of_fragment>
```
Cat can't really determine how to react on a fragment, so it saves it into buffer corresponfing to an IP of sender and send amusing response.  
Here is an example:  
![frag_console_dialogue](https://github.com/user-attachments/assets/f12ea73c-9017-47ae-8aa9-f039f79e405e)  
Traffic:  
![frag_request_exp](https://github.com/user-attachments/assets/d39549e3-0534-434f-9c22-50c70c3dfd95)  
![frag_response_exp](https://github.com/user-attachments/assets/61c00a36-f11f-48f5-a364-9af071e17e8b)  
full dump [here](https://disk.yandex.ru/d/S98YmRUaVea2fw)  




