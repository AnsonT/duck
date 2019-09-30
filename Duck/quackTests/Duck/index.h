const char MAIN_page[] PROGMEM = R"=====(

<!DOCTYPE html>
<html>
   <head>
      <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0" />
   </head>
   <style>
      body {
      --dark-color: black;
      --second-color: #ef0014;
      --main-color: #ef0014;
      --white-color: F5F5F5;
      --gradient-color: linear-gradient(to right, orange, var(--second-color));
      background: var(--white-color);
      font: 14px "Avenir", helvetica, sans-serif;
      -webkit-font-smoothing: antialiased;
      /*     --gradient-color: linear-gradient(to right, #f12711, #f5af19); */
      }
      .formDiv {
      position: relative;
      top: 0px;
      left: 0;
      width: 100%;
      height: 100%;
      text-align: center;
      font-weight: 300;
      z-index: 10;
      align-items: center;
      /*     overflow: scroll; */
      }
      .label-warning {
      color: var(--third-color);
      }
      .label-section {
      float: left;
      color: var(--main-color);
      font-weight: 600;
      }
      .form {
      width: 500px;
      background: var(--white-color);
      border-radius: 8px;
      margin:auto;
      padding: 25px 25px;
      max-width: calc(100vw - 40px);
      box-sizing: border-box;
      }
      input {
      width: 100%;
      padding: 10px;
      box-sizing: border-box;
      background: 0 0;
      outline: 0;
      resize: none;
      border: 0;
      color: var(--dark-color);
      font-size: 12px;
      }
      button {
      margin: 8px 0 0;
      border: 0px transparent;
      cursor: pointer;
      color: white;
      border-radius: 50px;
      padding: 12px 48px;
      font-family: "Nunito", sans-serif;
      font-size: 16px;
      font-weight: 200;
      text-decoration: none;
      background: #f12711;
      background: -webkit-linear-gradient(to left, #f5af19, #f12711);
      background: var(--gradient-color);
      background-position: 100% 0;
      background-size: 200% 200%;
      box-shadow: 0 16px 32px 0 rgba(255, 0, 0, 0.2);
      transition: all 0.5s ease;
      }
      button:hover {
      box-shadow: 0 0px 0px 0 rgba(0, 40, 120, 0);
      background-position: 0 0;
      }
      textarea,
      input.text,
      input[type="text"],
      input[type="button"],
      input[type="submit"],
      .input-checkbox {
      -webkit-appearance: none;
      border-radius: 0;
      }
      .cb {
      float: left;
      display: inline-block;
      color: red;
      cursor: pointer;
      position: relative;
      }
      .cb + .label-choice {
      cursor: pointer;
      color: black; /*var(--main-color); */
      display: block;
      padding: 10px;
      text-align: left !important;
      /* To keep the switch from getting misaligned */
      }
      .label-choice .sw {
      display: inline-block;
      position: relative;
      background-color: transparent;
      width: 25px;
      height: 25px;
      transform-origin: center;
      border: 2px solid var(--main-color);
      border-radius: 50%;
      vertical-align: -6px;
      margin-right: 10px;
      transition: background-color 150ms 200ms,
      transform 350ms cubic-bezier(0.78, -1.22, 0.17, 1.89);
      }
      .label-choice .sw:before {
      content: "";
      width: 0px;
      height: 2px;
      border-radius: 2px;
      background: var(--main-color);
      position: absolute;
      transform: rotate(45deg);
      top: 13px;
      left: 9px;
      transition: width 50ms ease 50ms;
      transform-origin: 0% 0%;
      }
      .label-choice .sw:after {
      content: "";
      width: 0;
      height: 2px;
      border-radius: 2px;
      background: var(--main-color);
      position: absolute;
      transform: rotate(305deg);
      top: 16px;
      left: 10px;
      transition: width 50ms ease;
      transform-origin: 0% 0%;
      }
      .label-choice:hover .sw:before {
      width: 5px;
      transition: width 100ms ease;
      }
      .label-choice:hover .sw:after {
      width: 10px;
      transition: width 150ms ease 100ms;
      }
      input[type="checkbox"] {
      display: none;
      }
      input[type="checkbox"]:checked + .label-choice .sw {
      background-color: var(--main-color);
      transform: scale(1.25);
      }
      input[type="checkbox"]:checked + .label-choice .sw:after {
      width: 10px;
      background: white;
      transition: width 150ms ease 100ms;
      }
      input[type="checkbox"]:checked + .label-choice .sw:before {
      width: 5px;
      background: white;
      transition: width 150ms ease 100ms;
      }
      input[type="checkbox"]:checked + .label-choice:hover .sw {
      background-color: red;
      transform: scale(1.25);
      }
      input[type="checkbox"]:checked + .label-choice:hover .sw:after {
      width: 10px;
      background: white;
      transition: width 150ms ease 100ms;
      }
      input[type="checkbox"]:checked + .label-choice:hover .sw:before {
      width: 5px;
      background: white;
      transition: width 150ms ease 100ms;
      }
   </style>
   <body onload='loadPage()'>
      <div class="page" id="page">
         <div class="formDiv">
            <form class="form" action='' method='get'>
               <p style='color:var(--main-color)'>DUCKLINK EMERGENCY PORTAL</p>
               <!--         <hr> -->
               <!--         <label class='label-warning' style='font-size: 0.8em; line-height:1.5em'> Wolf: Potential threat that poses a danger to an unknown degree</label> -->
               <hr>
               <!--         Replaced unique id with phone number -->
               
               <br><label class='label-section'>MY INFORMATION *</label>
                
               <input id='uuid' name='uuid' type='hidden' value="temp" />
               <input name='name' placeholder="Name"></input>
               <input name='street' placeholder="Street"></input>
               <input name="not_gonna_submit" disabled="disabled" value="City, State" />
               <input id='numberize' name='phone' placeholder="Phone Number"></input>
               <input type='number' name='occupants' placeholder="Number of occupants (include pets)"></input>
               
               <!--   CURRENT STATUS -->
               <br><br><br><label class='label-section'>CURRENT STATUS *</label><br>
               <input type="hidden" name='danger' value="0"><input type="checkbox" class="cb" id="a" onclick="this.previousSibling.value=1-this.previousSibling.value">
               <label class='label-choice' for="a"><span class="sw"></span>Are you in immediate danger?</label>
               <input type="hidden" name='vacant' value="0"><input type="checkbox" class="cb" id="b" onclick="this.previousSibling.value=1-this.previousSibling.value">
               <label class='label-choice' for="b"><span class="sw"></span>Is your house vacant?</label>
               
               <!-- I NEED -->
               <br><br><label class='label-section'>I NEED</label><br>
               <input type="hidden" name='firstaid' value="0"><input type="checkbox" class="cb" id="c" onclick="this.previousSibling.value=1-this.previousSibling.value">
               <label class='label-choice' for="c"><span class="sw"></span>First Aid</label>
               <input type="hidden" name='water' value="0"><input type="checkbox" class="cb" id="d" onclick="this.previousSibling.value=1-this.previousSibling.value">
               <label class='label-choice' for="d"><span class="sw"></span>Water</label>
               <input type="hidden" name='food' value="0"><input type="checkbox" class="cb" id="e" onclick="this.previousSibling.value=1-this.previousSibling.value">
               <label class='label-choice' for="e"><span class="sw"></span>Food</label>
               <input name='net' disabled type="checkbox" class="cb" id="f">
               <label class='label-choice' for="f"><span class="sw"></span>To watch Netflix</label>
               <br><br><label class='label-section'>MESSAGE US</label><br>
               <input name='message' placeholder="Please limit to 140 characters" maxlength="140"></input>
               
               <button type='submit' id='submit'>SOS</button>
            </form>
         </div>
      </div>
      <div class='page' id='page2' style="display:none;">
         <div class='formDiv'>
            <p style='color:var(--white-color)'>Thank you for your submission to the Clusterduck network!</p>
            <button type='button' id='goBack' onclick='goBack();'>GO BACK</button>
         </div>
      </div>
      <script>
         var number = document.getElementById('numberize');
         numberize.onchange = function() {
           numberize.value = numberize.value.replace(/[^0-9]/g, '');
         }

         
         function loadPage() {
             var url = window.location.href;
             var messageId = makeid();
         
             if(url.indexOf('name') > -1) {
               document.getElementById('page2').style = "display: block";
               document.getElementById('page').style = "display: none";
             }
             else {
               document.getElementById('page').style = "display: block";
               document.getElementById('page2').style = "display: none";
         
               var randomNumber = Math.floor((Math.random() * 1000000) + 1);
               document.getElementById('uuid').value = randomNumber;
             }
           }
         
           if (window.location.host == "captive.apple.com") {
             window.location = window.location.href.replace(
               "captive.apple.com",
               "duck.local/"
             );
           }
         
           function goBack() {
             window.history.back();
           }

           function makeid() {
             var text = "";
             var possible = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

             for (var i = 0; i < 12; i++)
             text += possible.charAt(Math.floor(Math.random() * possible.length));

             document.getElementById("uuid").value = text;

             return text;
           }
      </script>
   </body>
</html>

)=====";
