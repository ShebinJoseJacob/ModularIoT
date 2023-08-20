const static char home_0[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>WiFi Provision</title>
    <link href="https://fonts.googleapis.com/css?family=Poppins:400,600,700&display=swap" rel="stylesheet">
    <link href="https://fonts.googleapis.com/icon?family=Material+Icons" rel="stylesheet">

    <script src="https://kit.fontawesome.com/f0601b0fb2.js" crossorigin="anonymous"></script>
    <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body translate="no">
    <div class="container">
        <div class="components">
            <div class="page_title">
                Wi-Fi Provision Page
            </div>
            <form class="form" id="Form" action="/submit" method="GET">
                <input type="text" class="form__input" placeholder="WiFi SSID" name="ssid">
                <input type="password" class="form__input" placeholder="WiFi Password" name="password">
            </form>
            <div class="btn btn__primary" onclick="submitForm()">
                <p>Submit</p>
            </div>
</body>
</html>
)=====";

const static char response_0[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>WiFi Provision</title>
    <link href="https://fonts.googleapis.com/css?family=Poppins:400,600,700&display=swap" rel="stylesheet">
    <link href="https://fonts.googleapis.com/icon?family=Material+Icons" rel="stylesheet">

    <script src="https://kit.fontawesome.com/f0601b0fb2.js" crossorigin="anonymous"></script>
    <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body translate="no">
    <div class="container">
        <div class="components">
            <div class="page_title">
                You're All Set!
            </div>
  
          </div>
</body>
</html>
)=====";

const static char home_1[] PROGMEM = R"=====(
    <style>
           :root {
      --primary-light: #8abdff;
      --primary: #6d5dfc;
      --primary-dark: #5b0eeb;
      --white: #FFFFFF;
      --greyLight-1: #E4EBF5;
      --greyLight-2: #c8d0e7;
      --greyLight-3: #bec8e4;
      --greyDark: #9baacf;
  }
  
  *,
  *::before,
  *::after {
      margin: 0;
      padding: 0;
      box-sizing: inherit;
  }
  
  html {
      box-sizing: border-box;
      font-size: 62.5%;
      overflow-y: scroll;
      background: var(--greyLight-1);
  }
  @media screen and (min-width: 900px) {
      html {
          font-size: 75%;
      }
  } 
  .container {
      min-height: 100vh;
      display: flex;
      justify-content: center;
      align-items: center;
      font-family: "Poppins", sans-serif;
      background: var(--greyLight-1);
  }
  .components {
      width: 60%;
      height: 35rem;
      border-radius: 3rem;
      box-shadow: 0.8rem 0.8rem 1.4rem var(--greyLight-2), -0.2rem -0.2rem 1.8rem var(--white);
      padding: 4rem;
      display: flex;
      align-items: center;
      flex-direction: column;
  }
)=====";

const static char home_2[] PROGMEM = R"=====(
  .page_title{
    font-size: 1.8rem;
    color: var(--primary);
    margin-bottom: 3rem;
  }
  /*  BUTTONS  */
  
  .btn {
      width: 13rem;
      height: 4rem;
      border-radius: 1rem;
      box-shadow: 0.3rem 0.3rem 0.6rem var(--greyLight-2), -0.2rem -0.2rem 0.5rem var(--white);
      justify-self: center;
      display: flex;
      align-items: center;
      justify-content: center;
      cursor: pointer;
      transition: 0.3s ease;   
  }
  
  .btn__primary {
      background: var(--primary);
      box-shadow: inset 0.2rem 0.2rem 1rem var(--primary-light), inset -0.2rem -0.2rem 1rem var(--primary-dark), 0.3rem 0.3rem 0.6rem var(--greyLight-2), -0.2rem -0.2rem 0.5rem var(--white);
      color: var(--greyLight-1);
      
  }
  
  .btn__primary:hover {
      color: var(--white);
  }
  
  .btn__primary:active {
      box-shadow: inset 0.2rem 0.2rem 1rem var(--primary-dark), inset -0.2rem -0.2rem 1rem var(--primary-light);
  }
  
  .btn p {
      font-size: 1.6rem;
  }
  
  /*  FORM  */
  
  .form {
      grid-column: 3/4;
      grid-row: 3/4;
  }
  )=====";
  const static char home_3[] PROGMEM = R"=====(
  .form__input {
      width: 20.4rem;
      height: 4rem;
      border: none;
      border-radius: 1rem;
      font-size: 1.4rem;
      padding-left: 1.4rem;
      box-shadow: inset 0.2rem 0.2rem 0.5rem var(--greyLight-2), inset -0.2rem -0.2rem 0.5rem var(--white);
      background: none;
      font-family: inherit;
      color: var(--greyDark);
      margin-bottom: 2rem;
  }
  
  .form__input::-moz-placeholder {
      color: var(--greyLight-3);
  }
  
  .form__input:-ms-input-placeholder {
      color: var(--greyLight-3);
  }
  
  .form__input::placeholder {
      color: var(--greyLight-3);
  }
  
  .form__input:focus {
      outline: none;
      box-shadow: 0.3rem 0.3rem 0.6rem var(--greyLight-2), -0.2rem -0.2rem 0.5rem var(--white);
  }
    </style>
    <script>
        function submitForm(){
            const Form = document.getElementById('Form');
            Form.submit();
        }
    </script>
</body>

</html>
)=====";
