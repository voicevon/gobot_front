// =====================
//HTML code for webpage
//=====================
const char webpageCode[] PROGMEM =
R"=====(
<!DOCTYPE HTML>
<html>
<head>
  <title>Convolution Integral PTE.LTD.</title>
  <meta charset="utf-8"> 
</head>
<!-------------------------------C S S------------------------------>
<style>
  #btn
  {
    display: inline-block;
    text-decoration: none;
    background: #8CD460;
    color: rgba(255,255,255, 0.80);
    font-weight: bold;
    font: 60px arial, sans-serif;
    width: 150px;
    height: 150px;
    line-height: 150px;
    border-radius: 50%;
    text-align: center;
    vertical-align: middle;
    overflow: hidden;
    box-shadow: 0px 0px 0px 8px #8CD460;
    border: solid 2px rgba(255,255,255, 0.47);
    transition: 0.5s;
  }
  body {text-align:center; font-family:"Calibri"; background-color:rgba(0, 3, 8, 0.26)}
  h1   {color: rgba(0, 0, 255, 0.87); font-size: 50px;}
</style>
<!------------------------------H T M L----------------------------->
<body>
   <h1 id="title">生物实验室-模拟呼吸机</h1>
   <h2>已执行次数(次)： <span id = "done_count">0</span> 
   <button onclick="onReset()">复位</button></h2>
  <!-- 设定次数: <span id = "total_count">0</span>   <p></p></p></h2> -->
  <table align="center">
    <tr>
      <td align="right">设定总次数(次)：</td>
      <td><input type="text" maxlength="3" size ="3" value="300" id="total_count"></td>
      <td> <button onclick="onTotalCount()">提交</button></td>
    </tr>
    <tr>
      <td align="right">单次抽吸体积(ml)：</td>
      <td><input type="text" maxlength="3" size ="3" value="55" id="per_volume"></td>
      <td><button onclick="onPerVolume()">提交</button></td>
    </tr>
    <tr>
      <td align="right">单次抽吸时间(s)：</td>
      <td><input type="text" maxlength="3" size ="3" value="3" id="per_pull_in_second"></td>
      <td><button onclick="onPerPullTime()">提交</button></td>
    </tr>
    <tr>
      <td align="right">单次等待时间(s)：</td>
      <td><input type="text" maxlength="3" size ="3" value="30" id="per_sleep_in_second"></td>
      <td><button onclick="onPerSleepTime()">提交</button></td>
    </tr>
  </table>
  <hr>

    <br><p><p>
    <a href="#" id="btnOnOff" ONCLICK='btn_OnOff()'> On-Off</a>
    <p><p><p>
    设计和制造：<br>
    山东卷积分公司 2021<br>
    冯旭明, 电话：133-0640-0990<br> 

<!-----------------------------JavaScript--------------------------->
  <script>
     var first_load = true;
     InitWebSocket()
     function InitWebSocket()
     {
       websock = new WebSocket('ws://'+window.location.hostname+':81/'); 
       websock.onmessage = function(evt)
       {
          JSONobj = JSON.parse(evt.data);
          message = JSONobj
          document.getElementById("title").innerHTML = message;


          document.getElementById('btnOnOff').innerHTML = JSONobj.varOnOff;
          if(JSONobj.varOnOff == 'ON'){
            document.getElementById('btnOnOff').style.background='#FF0000';
            document.getElementById('btnOnOff').style["boxShadow"] = "0px 0px 0px 8px #FF0000";
          }else{
            document.getElementById('btnOnOff').style.background='#111111';
            document.getElementById('btnOnOff').style["boxShadow"] = "0px 0px 0px 8px #111111";
          }


          
          document.getElementById('done_count').innerHTML = JSONobj.var_done_count;
          if (first_load){
            first_load = false;
            total_count.value = JSONobj.var_total_count;
            document.getElementById('volume').value = JSONobj.var_volume;
            document.getElementById('volume_output').innerHTML = JSONobj.var_volume;
            document.getElementById('push_time').value = JSONobj.var_push_time;
            document.getElementById('push_output').innerHTML = JSONobj.var_push_time;
            document.getElementById('sleep_time').value = JSONobj.var_sleep_time;
            docement.getElementById('sleep_output').innerHTML = JSONobj.var_sleep_time;
          }
       }
     }
     //-------------------------------------------------------------
     function onReset(){
       payload = "total_count=0";
       document.getElementById("title").innerHTML = payload;
       websock.send(payload);
     }

     function onTotalCount(){
       payload = "total_count=" + document.getElementById("total_count").value;
       document.getElementById("title").innerHTML = payload;
       websock.send(payload);
    }
    function onPerVolume(){
       payload = "per_volume=" + document.getElementById("per_volume").value;
       document.getElementById("title").innerHTML = payload;
       websock.send(payload);
    }
    function onPerPullTime(){
       payload = "per_pull_in_second=" + document.getElementById("per_pull_in_second").value;
       document.getElementById("title").innerHTML = payload;
       websock.send(payload);
    }
    function onPerSleepTime(){
       payload = "per_sleep_in_second=" + document.getElementById("per_sleep_in_second").value;
       document.getElementById("title").innerHTML = payload;
       websock.send(payload);
    }
     function btn_OnOff()
     {
        payload = 'varOnOff=ON';
        if(document.getElementById('btnOnOff').innerHTML == 'ON'){
            payload = 'varOnOff=OFF';
        }
        websock.send(payload);
     }


  </script>
</body>
</html>
)=====";
