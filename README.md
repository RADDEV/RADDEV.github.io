# GitHub Pages Firebase Project

This project implements a static website hosted on GitHub Pages that connects to a Firebase database. It features a button that interacts with the database by setting a value to 1 when pressed down and to 0 when released.

## Project Structure

```
github-pages-firebase-project
├── public
│   ├── index.html        # Main HTML document
│   ├── styles
│   │   └── style.css     # Styles for the website
│   └── scripts
│       └── app.js        # JavaScript for Firebase connection and button events
├── .firebaserc           # Firebase project configuration
├── firebase.json         # Firebase Hosting configuration
└── README.md             # Project documentation
```

## Setup Instructions

1. **Clone the Repository**
   Clone this repository to your local machine using:
   ```
   git clone <repository-url>
   ```

2. **Install Firebase CLI**
   Make sure you have the Firebase CLI installed. If not, you can install it using npm:
   ```
   npm install -g firebase-tools
   ```

3. **Configure Firebase**
   Update the `.firebaserc` file with your Firebase project ID and aliases.

4. **Deploy to Firebase Hosting**
   Navigate to the project directory and run:
   ```
   firebase deploy
   ```

## Usage

- Open the website in your browser.
- Press the button to set the value to 1.
- Release the button to set the value back to 0.

## License

This project is licensed under the MIT License.