/*
The function takes input of the string and inverts it according to the Atbash Cipher Algorithm

Algorithm Description:

For all the english latin letter it inverts its value according to the position in english alphabet.
Therefore, a -> z, b -> y, .. z -> a

For all other letters, they are left unchanged
*/

function invertString(input) {
	let answer = "";
	for(let i = 0; i < input.length; ++i) {
		let code = input[i].charCodeAt();
		if(input[i] >= 'a' && input[i] <= 'z') {
			answer += String.fromCharCode(219 - code);
		}
		else if(input[i] >= 'A' && input[i] <= 'Z') {
			answer += String.fromCharCode(155 - code);
		}
		else {
			answer += input[i];
		}
	} 
	return answer;
}

/*

Add event handlers to listen to input change events and update the corresponding text
*/
var input = document.querySelector("#input");
var output = document.querySelector("#output");

input.addEventListener("input", (e) => {
	output.value = invertString(e.target.value);
});

output.addEventListener("input", (e) => {
	input.value = invertString(e.target.value);
});