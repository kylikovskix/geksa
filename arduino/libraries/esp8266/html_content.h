﻿#ifndef HTML_CODE_H
#define HTML_CODE_H
 
#define SETUP_HTML_CONTENT \
"<html>"\
"<head>"\
"<meta charset='utf-8'/>"\
"</head>"\
"<body>"\
"<div align='center' valign='midle' higth='100%' style='background: radial-gradient(40% 50%, #FAECD5, #CAE4D8);'>"\
 "<h1>Настройка сетевых параметров</h1>"\
 "<hr>"\
"<form method=post>"\
" <table border=1>"\
"  <tr>"\
"   <td align='right'>New SSID:</td>"\
"   <td><input name='ssid'></td>"\
"  </tr>"\
"  <tr>"\
"   <td align='right'>New password:</td>"\
"   <td><input name='npass' type='password'></td>"\
"  </tr>"\
"  <tr>"\
"   <td align='right'>Current password:</td>"\
"   <td><input name='cpass' type='password'></td>"\
"  </tr>"\
" </table>"\
" <hr>"\
" <input type='submit'>"\
"</form>"\
"</div>"\
"</body>"\
"</html>"

#endif