#include "Arduino.h"
#include <WString.h>
#include "format_html.h"

String format_page_web(){
  String s = "HTTP/1.1 200 OK\r\n";
          s += "Content-Type: text/html\r\n\r\n";
          s += "<!DOCTYPE HTML>\r\n<html>\r\n";

          s += "<script type=\"text/javascript\">";
          s += "function addcolor(){";
         
          s += "var rouge = document.getElementById('rouge_id_rgb').value;";
          s += "var vert = document.getElementById('vert_id_rgb').value;";
          s += "var bleu = document.getElementById('bleu_id_rgb').value;";

          s += "";
          
          s += "var req = '1xrgb-r' + rouge + '.g' + vert + '.b' + bleu + '-END';";
          s += "document.location.href=req;";
          s += "}";
          s += "</script>";


          s += "<script type=\"text/javascript\">";
          s += "function setcolor(){";
          s += "var rouge = document.getElementById('rouge_id_rgb').value;";
          s += "var vert = document.getElementById('vert_id_rgb').value;";
          s += "var bleu = document.getElementById('bleu_id_rgb').value;";
          s += "var req = 'color-r' + rouge + '.g' + vert + '.b' + bleu + '-END';";
          s += "document.location.href=req;";
          s += "}";
          s += "</script>";

          
          s += "<br><input type=\"button\" name=\"bl\" value=\"ON Bas -> Haut\" onclick=\"location.href='/0xF7C03F'\">";
          s += "<br>";
          s += "<br><input type=\"button\" name=\"bl\" value=\"OFF Bas -> Haut\" onclick=\"location.href='/0xF740BF'\">";
          s += "<br>";

          s += "<br><input type=\"button\" name=\"bl\" value=\"ON Haut -> Bas\" onclick=\"location.href='/0xZ7Z93R'\">";
          s += "<br>";
          s += "<br><input type=\"button\" name=\"bl\" value=\"OFF Haut -> Bas\" onclick=\"location.href='/0xF7T53R'\">";
          s += "<br>";

          s += "<br><input type=\"button\" name=\"bl\" value=\"test \" onclick=\"location.href='/0xtest03F'\">";
          s += "<br>";

          
          s += "<br><input type=\"button\" name=\"bl\" value=\"Start Fading \" onclick=\"location.href='/1xstartfade'\">";
          s += "<br>";
          s += "<br><input type=\"button\" name=\"bl\" value=\"Stop Fading \" onclick=\"location.href='/1xstopfade'\">";
          s += "<br>";
          s += "<br>";

          s += "<input type='text' id='rouge_id_rgb' value='Rouge' />";
          s += "<input type='text' id='vert_id_rgb' value='Vert' />";
          s += "<input type='text' id='bleu_id_rgb' value='Bleu' />";
          s += "<br><input type=\"button\" name=\"bl\" value=\"Add Color RGB to list \" onclick=\"addcolor();\">";

       
          s += "<br><input type=\"button\" name=\"bl\" value=\"Set Color \" onclick=\"setcolor();\">";
          s += "<br>";

      
          s += "<br><input type=\"button\" name=\"bl\" value=\"Clear List Fade \" onclick=\"location.href='/1xclear'\">";
          s += "<br>";
         
          
          s += "<br><input type=\"button\" name=\"bl\" value=\"plus lumineux\" onclick=\"location.href='/0xF700FF'\">";
          s += "<br>";
          s += "<br><input type=\"button\" name=\"bl\" value=\"plus foncé\" onclick=\"location.href='/0xF7807F'\">";

          s += "<br>";
          s += "<br><input type=\"button\" name=\"bl\" value=\"plus lent\" onclick=\"location.href='/0xF7D02F'\">";
          s += "<br>";
          s += "<br><input type=\"button\" name=\"bl\" value=\"plus rapide\" onclick=\"location.href='/0xF7F00F'\">";

          s += "<br><br><input type=\"button\" name=\"bl\" value=\"Fade Massage\" onclick=\"location.href='/0xF7E817'\">";
          s += "<br><input type=\"button\" name=\"bl\" value=\"Fade Relax\" onclick=\"location.href='/0xD9ER18'\">";


          s += "</html>\n";

          return s;
}
