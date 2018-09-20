For my toy DES program I wrote simple encryption and decryption algo. They take in the plaintext and memory to write
the ciphertext to. Then they will encrypt block by block. I realized that the difference between encrypt and decrypt
was the key order. So in my implementation I just switch key order and pass it into the block for decrypt versus encrypt.
 My program also has lots of helper function to do left circular shifts, permutations, and the f box stuff. On start up
my program take a key, ip to send file, port, and a file. It will then open read the contents of the file and send it to
the specified IP and port. It will of course encrypt it before send. I also will encrypt and decrypt a text message that
to show that it is working. It encrypts it prints the hex to screen then decrypts it and prints it out. I also have
written the server the will recv the file from small client I wrote. It takes in the key and then will recv a file and
save the file as encrypted and also decrypted. The file names that it saves our always test_file, which has decrypted
file, and test_file_e, which has encrypted file. This is so you can see that it is encrypting and sending a file then
decrypting it. Now this only will only really work with one file at a time as it closes after file send happens.
It could however be extended to work so you can send multiple files. Could even be used to encrypt stuff as a chat
server and send messages between friends. If you want to compile my program you need to compile the reciver.c, and
the des.c separately. To make this easier I have made a make file. Run make and you will have a dec, and reciver
files to run. Reciver file is server that recv the file and the des will send any file to it. Make sure you have two
empty files called test_file, and test_file_e for it to work properly. In c all of the bit operation I have done by
lots of bit shift and bit operations. I see why I was told not to do C, but I love C. In my main function I generate
the two part of the key, using left circular shifts and permutations. The keys are then used in the encryption and
decryption algorithms. My box takes in the keys and will do permutation to expand to 8 bits. It then does xor runs
through the f box xor and run other side through it. This gives you your 8 bit result. Also a problem with this
encryption is that we are doing 8 bit values. This means each char is replaced with different char. That makes it
similar to a substitution cipher. We need to use larger blocks to make it more secure. That is why normal encryption
uses 16 byte key or even bigger.


Examlple running it:
`./reciver 123`

`./des 123 127.0.0.1 1234 des.c`

Then the file test_file should be the same as des.c after being sent as encrypted file
