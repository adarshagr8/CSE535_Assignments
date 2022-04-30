/*

Name: Adarsh Agrawal
Roll: 18075005
Branch: CSE

*/

#include <bits/stdc++.h>
using namespace std;


/*

	El Gamal Algorithm:

	It is an asymmetric encryption algorithm where there are two keys (public and private),
	public key is used for encryption and the private key is used for decryption

	Suppose Bob and Alice wants to communicate, Bob wants to send message to Alice.
	Bob would randomly select two public parameters:
	- p: A large enough prime number
	- g: A generator in the range [1, p)

	Subsequent procedure described in main()

*/

const long long int p = 309427398372577LL;
const long long int g = 245612351365707LL;


/*

	power() function calculates base ^ expo % modulo in logarithmic complexity

*/

long long int power(long long int base, long long int expo, const long long int modulo) {
	if(expo == 0) {
		return 1LL;
	}
	__int128 ans = power(base, expo / 2, modulo);
	ans = (ans * ans) % modulo;
	if(expo & 1) {
		ans = (ans * base) % modulo;
	}
	return ans;
}

/*
	Function to print 128 byte integers
*/
std::ostream& operator<<(std::ostream& o, const __int128& x) {
    if (x == std::numeric_limits<__int128>::min()) return o << "-170141183460469231731687303715884105728";
    if (x < 0) return o << "-" << -x;
    if (x < 10) return o << (char)(x + '0');
    return o << x / 10 << (char)(x % 10 + '0');
}


/*

	Class to simulate the behaviour of person involved in communication

*/

class Person {
	const long long int private_key;
	const long long int public_key;
public:
	Person(long long int private_key, long long int public_key): private_key(private_key), public_key(public_key) {
		std::cout << "Generated Private Key: " << private_key << std::endl;
		std::cout << "Generated Public Key: " << public_key << std::endl;
		std::cout << std::endl;
	}

	long long int getPublicKey() const {
		return public_key;
	}

	/* 
		sendMessage() function takes the message and rvalue reference to the recipient
		and generates the encrypted message
	*/
	__int128 sendMessage(const long long int &message, const Person &recipient) {
		long long int recipients_public_key = recipient.getPublicKey();
		auto key = power(recipients_public_key, private_key, p);
		__int128 encrypted_message = (__int128) message * key;
		return encrypted_message;
	}

	/*
		receiveMessage() function takes the encrypted message and rvalye reference
		to the sender of the message and decrypts it
	*/
	long long int receiveMessage(__int128 encryptedMessage, const Person &sender) {
		encryptedMessage %= p;
		auto senders_public_key = sender.getPublicKey();
		__int128 temp = power(senders_public_key, private_key, p);
		/*
			Fermat's little theorem
		*/
		temp = power(temp, p - 2, p);
		temp = (temp * (encryptedMessage % p));
		long long int message = temp % p;
		return message;
	}
};

int main() {

	/*

		Bob and Alice both would randomly pick a private key in the range [0, p-2]

	*/

	std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());

	const long long int bob_private_key = rng() % (p - 1);
	const long long int alice_private_key = rng() % (p - 1);

	/*

		Bob and Alice would now calculate their public keys which are given as
		g ^ (private_key) mod p

	*/

	const long long int bob_public_key = power(g, bob_private_key, p);
	const long long int alice_public_key = power(g, alice_private_key, p);

	/*
	
		We will create Person class objects

	*/

	Person Bob(bob_private_key, bob_public_key);
	Person Alice(alice_private_key, alice_public_key);

	/*
		Testing
	*/

	for(int i = 0; i < 5; i++) {

		long long int random_message = rng() % p;

		std::cout << "Random Message: " << random_message << std::endl;

		__int128 encrypted_message = Bob.sendMessage(random_message, Alice);

		std::cout << "Encrypted Message: " << encrypted_message << std::endl;

		auto decrypted_message = Alice.receiveMessage(encrypted_message, Bob);

		std::cout << "Decrypted Message: " << decrypted_message << std::endl;
		
		std::cout << "Test Passed!" << std::endl << std::endl;

		assert(decrypted_message == random_message);
	}

}

