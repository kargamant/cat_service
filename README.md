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

## Fragmentation  
If a client has a very long nickname that does not fit in buff_size, then several UDP fragments will be sent.  
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
  
## DB integration  
To keep the service simple, a single file was introduced as a database.  It loads into program when server starts and offloads after every response.  
If a user succesfully fed the cat then ``` user 0 ``` will be written in cat_log.txt file. If not then ``` user 1 ```.  
Example:  
![cat_log_db](https://github.com/user-attachments/assets/9da1f5fc-2d06-4caa-bca7-a575dc7a1d2f)  

# Pet the cat service  

The cat also likes to play. But not with all users. The chance of succesful play with cat is ```total_succesful_requests/total_requests```.  
If a play was succesful 0 is written to DB for that user. If not then 2 is written to DB for that user. Also there is a tiny 5 percent chance that cat will fall asleep and not respond anymore.  
The only data sent is nickname of a user.  

Requests and responses for that service are sent via TCP. So prior connection between server and client is established.  
Here is a basic communication:  
![tcp_console_dialogue](https://github.com/user-attachments/assets/9aea538d-b4f4-4e5a-86b2-f6196d3c6ea2)  
Traffic:  
![tcp_request_exp](https://github.com/user-attachments/assets/ea4b5a06-f24d-452e-afc3-a5fabda84967)  

![tcp_tol_response](https://github.com/user-attachments/assets/af532c74-b1ac-4a9f-ac93-2753f4cc30d3)  

![tcp_bite_response](https://github.com/user-attachments/assets/a66f5bc5-d24e-4e12-9e93-abe4631c9a86)  
![tcp_bored_response](https://github.com/user-attachments/assets/ab0da28f-1ec1-4920-8754-e1b9066b4371)  
> Worth noticing that RST is sent when cat falls asleep. Pretty much like in real life.
  
full dump [here](https://disk.yandex.ru/d/Upa0I-zkK_oGEw)  

## Sequence requests  
TCP data is a stream divided in segments, so segments can contain any portion of data.  
Therefore it was decided to make fragmenting mechanisms and ability to respond to a sequence of requests in one segment.  
Sequence request is something like following:  
```
@123~@aboba~@Vladimir777~
```
It can contain any number of nicknames.  So when cat recieves this sequence it iterates through all nickname matches and give it response according to database.  
Then All responses are accumulated into one big response with type Default.  But importantly, if one of responses was Sleep response(cat is tired) then after sending this big accumulated response connection is closing.  

Here is an example:  
![stream_tcp_console_dialogue](https://github.com/user-attachments/assets/05c572f3-fa8c-4799-a54a-453aa352277c)  
full dump [here](https://disk.yandex.ru/d/bjWgIqtEBRvlkw)  

## Fragmentation  
If a nickname is very big, then it should be sent in separate segments and collected properely on server side. For that purpose server has separate buffer for every IP it handles communication with.  
So cat will collect all segments until it finds at least a submatch. If a submatch in a stream was found then the response will be sent and buffer flushed(for more details you can see the Orchestrator class in code).  
Here is an example:  
![frag_tcp_console_dialogue](https://github.com/user-attachments/assets/6df49dcc-8483-4e54-8dc4-ae9a1af16835)  
full dump [here](https://disk.yandex.ru/d/DbntEKClPwxVqQ)  

## Sequence fragmentation💀  
If a sequence request does not fit in buff_size... It should be splitted into several segments :)  
The mechanism is the same as for fragmentation of one request. Segments are gathered into buffer, accumulated and if there is a match, response is given.  
Example:  
![multiple_stream_tcp_console_dialogue](https://github.com/user-attachments/assets/dfebc654-6dfc-4e44-a4b2-0eaf84cbd6d6)  
full dump [here](https://disk.yandex.ru/d/48Jgu_moeIcp8w)  

## DB updates  
In TCP service if a client succesfully plays with a cat it gets 0 and unsuccesful attempt give client 2 points.  
cat_log.txt example:  
![image](https://github.com/user-attachments/assets/9bee0ff6-a239-4918-a780-d53e224e316d)  

# Build  
### requirements  
```
windows system that supports winsock2  
cmake 3.28+  
winsock2  
ninja 1.12+
```
In order to build run the following.  
```
$ cmake.exe -S . -B build
$ cd build
$ ninja.exe
```
> Actually, it is worth trying to build it with building systems other than ninja, but I personally use it.
> Also, it is worth trying to rewrite it for linux since linux does not support winsock2 :)
  
All dumps can be found [here](https://disk.yandex.ru/d/KM6fBp7XFxZS0Q)

# Analysis of communication  
These communication types have some dissadvantages. 

**Feed the cat**. UDP is not very reliable. What if a fragment of request will be lost? On server side if an end of request(ends with tilda is recieved) then cat responds.  
But it will respond with a format error if a fragment is not delivered. Moreover no authentication mechanism working. So any user can send request from any user_name and spoil score in database.  

**Pet the cat**. TCP is more reliable, but it also have some problems related to stream interpretation here. Server doesn't know anything about full stream data. It recieves only segments. Therefore cat is not only responding slowly due to accumulating nickname data in buffers, but also can respond incorrectly if some nickname fall on a border of two segments. First half will be flushed after response and other one remain.  






