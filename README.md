# epirandom
This project is a kernel object which use a device file to generate an infinite chain of characters.
If you try to read ```/dev/epirandom``` with ```cat``` for example, you will get a random characters.

## Usage
We assume that you have ```sudo``` ,```mknod``` and ```make``` installed in your system.

Run ```make``` to build the kernel object and generate ```epirandom.ko``` file.
Then ```sudo insmod ./epidrandom.ko alphabet=$(ALPHABET)```. Replace ```$(ALPHABET)``` with any suite of characters you want. If you don't want to use alphabet, just leave it empty. For example, ```sudo insmod epirandom.ko```, in this case you will get the same thing as ```/dev/urandom```.

Now your kernel object is loaded, but we don't have the ```/dev/epirandom``` device file yet. So we need to create it.
So to create it, you have to run ```make nod```. It will create the file script ```.major``` that contains the major number of the device file and the mknod command.
Then you can run ```bash .major``` to create the device file. (you can run the script with zsh if you prefer)

[Warning](#){.btn .btn-warning} you have to run ```bash .major``` like it is done in the example below, specifying the shell.
If you don't, the device file will not be created.
```bash
❯ bash
23:14:13 sabir:~/Epita/Ing1/sys2/epirandom $? $ bash .major
``` 

Now you can use ```cat /dev/epirandom``` to read the device file and see the magic 😉.

You Like it !!? Then you can run ```make clean``` to clean the kernel object and the device file. Don't forget to do it.

## Exceptionnal usage
* You can use dd to read the device file like you can see in the example below.
```bash
❯ bash
23:14:13 sabir:~/Epita/Ing1/sys2/epirandom $? $ dd if=/dev/epirandom of=epirandom.txt bs=512 count=1
``` 
you will copy the first 512 bytes of the device file to the file epirandom.txt.
* We handler an exceptionnal case if you want to separate the alphabet with a comma. See the example below.
```bash
❯ bash
23:14:13 sabir:~/Epita/Ing1/sys2/epirandom $? $ sudo insmod ./epirandom.ko alphabet="a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z"
``` 
The program will consider that the comma is a separator and not a character.
[Warning](#){.btn .btn-warning} **It work only with comma**.

Don't forget to clean the kernel object and the device file with ```make clean```
