//const ip = "10.8.215.228"
const ip = location.host
const url=`http://${ip}/api/move`;

var pressedKeys = {};
window.onkeyup = function(e) { pressedKeys[e.key] = false; }
window.onkeydown = function(e) { pressedKeys[e.key] = true; }

// set these to false so they're not undefined
pressedKeys["ArrowLeft"] = false;
pressedKeys["ArrowRight"] = false;
pressedKeys["ArrowUp"] = false;
pressedKeys["ArrowDown"] = false;

var intervalId = window.setInterval(function(){
	mainFunction()
}, 3000);


function mainFunction() {
	let data = {
		left: pressedKeys["ArrowLeft"],
		right: pressedKeys["ArrowRight"],
		up: pressedKeys["ArrowUp"],
		down: pressedKeys["ArrowDown"],
		speed: 1.0
	};
	console.log(data);

	fetch(url, {
		method: "POST",
		headers: {'Content-Type': 'application/json'},
		body: JSON.stringify(data)
	}).then(res => {
		console.log("Request complete! response:", res);
	});
}
