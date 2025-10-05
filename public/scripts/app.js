// Import Firebase core and specific modules
import { initializeApp } from "https://www.gstatic.com/firebasejs/9.23.0/firebase-app.js";
import { getDatabase, ref, set } from "https://www.gstatic.com/firebasejs/9.23.0/firebase-database.js";

// Initialize Firebasesss
const firebaseConfig = {
    apiKey: "AIzaSyAGXhj8jdFp9NIxzPdsVODeSO82AHeeiaw",
  authDomain: "onebutton-3a41a.firebaseapp.com",
  databaseURL: "https://onebutton-3a41a-default-rtdb.europe-west1.firebasedatabase.app",
  projectId: "onebutton-3a41a",
  storageBucket: "onebutton-3a41a.firebasestorage.app",
  messagingSenderId: "158710832948",
  appId: "1:158710832948:web:16dec6f87a8f905c8e8f61"
  };

// Initialize Firebase
const app = initializeApp(firebaseConfig);

// Reference to the database
const database = getDatabase(app);

// Get the button element
const button = document.getElementById('controlButton');

// Function to set value to 1
function setValueToOne() {
    console.log(1)
    set(ref(database, 'buttonState'), 1);
}

// Function to set value to 0
function setValueToZero() {
    console.log(0)
    set(ref(database, 'buttonState'), 0);
}

// Event listeners for button press
button.addEventListener('mousedown', setValueToOne);
button.addEventListener('mouseup', setValueToZero);
button.addEventListener('mouseleave', setValueToZero); // Handle case when mouse leaves button while pressed
button.addEventListener("touchstart", setValueToOne);
button.addEventListener("touchend", setValueToZero);