$(function(){
$('.tree li:has(ul)').addClass('parent_li').find(' > span').attr('title', 'Collapse this branch');
$('.tree li.parent_li > span').on('click', function (e) {
    var children = $(this).parent('li.parent_li').find(' > ul > li');
    if (children.is(":visible")) {
        children.hide('fast');
        $(this).attr('title', 'Expand this branch').find(' > i').addClass('icon-plus-sign').removeClass('icon-minus-sign');
    } else {
        children.show('fast');
        $(this).attr('title', 'Collapse this branch').find(' > i').addClass('icon-minus-sign').removeClass('icon-plus-sign');
    }
    e.stopPropagation();
});
});

function showCanva(number,canva){
    var c = document.getElementById(canva);
    var ctx = c.getContext("2d");
    ctx.clearRect(0,0,1000,1000);
    ctx.font = "100px Arial";
    ctx.fillStyle="#1874CD";
    ctx.fillText(number,70,120);
}
function showCanva2(number,canva){
    var c = document.getElementById(canva);
    var ctx = c.getContext("2d");
    ctx.clearRect(0,0,1000,1000);
    ctx.font = "100px Arial";
    ctx.fillStyle="#1874CD";
    ctx.fillText(number,110,120);
}
function showCanva3(number,canva){
    var c = document.getElementById(canva);
    var ctx = c.getContext("2d");
    ctx.clearRect(0,0,1000,1000);
    ctx.font = "100px Arial";
    ctx.fillStyle="#1874CD";
    ctx.fillText(number,42,120);
}
function mydatanote(d1,d2,d3,d4,start,now)
{
this.d1=d1;
this.d2=d2;
this.d3=d3;
this.d4=d4;
this.start=start;
this.now=now;
this.show=show;
this.clean=clean;
function show()
{
if(start<14)
{
$("#datanote"+(start)+"-"+1).append(now);
$("#datanote"+(start)+"-"+2).append("Temperature");   
$("#datanote"+(start)+"-"+3).append(d1);   
$("#datanote"+(start)+"-"+4).append("&#176;C");   
$("#datanote"+(start)+"-"+5).append("Manual"); 
$("#datanote"+(start+1)+"-"+1).append(now);
$("#datanote"+(start+1)+"-"+2).append("Tumidity");   
$("#datanote"+(start+1)+"-"+3).append(d2);   
$("#datanote"+(start+1)+"-"+4).append("%");   
$("#datanote"+(start+1)+"-"+5).append("Manual"); 
$("#datanote"+(start+2)+"-"+1).append(now);
$("#datanote"+(start+2)+"-"+2).append("Brightness");   
$("#datanote"+(start+2)+"-"+3).append(d3);   
$("#datanote"+(start+2)+"-"+4).append("cd/m^2");   
$("#datanote"+(start+2)+"-"+5).append("Manual"); 
$("#datanote"+(start+3)+"-"+1).append(now);
$("#datanote"+(start+3)+"-"+2).append("PM2.5");   
$("#datanote"+(start+3)+"-"+3).append(d4);   
$("#datanote"+(start+3)+"-"+4).append("&#956;g/m^3");   
$("#datanote"+(start+3)+"-"+5).append("Manual"); 
}
}
function clean()
{
$("#datanote"+(start)+"-"+1).empty();
$("#datanote"+(start)+"-"+2).empty();   
$("#datanote"+(start)+"-"+3).empty();   
$("#datanote"+(start)+"-"+4).empty();   
$("#datanote"+(start)+"-"+5).empty(); 
$("#datanote"+(start+1)+"-"+1).empty();
$("#datanote"+(start+1)+"-"+2).empty();   
$("#datanote"+(start+1)+"-"+3).empty();   
$("#datanote"+(start+1)+"-"+4).empty();   
$("#datanote"+(start+1)+"-"+5).empty(); 
$("#datanote"+(start+2)+"-"+1).empty();
$("#datanote"+(start+2)+"-"+2).empty();   
$("#datanote"+(start+2)+"-"+3).empty();   
$("#datanote"+(start+2)+"-"+4).empty();   
$("#datanote"+(start+2)+"-"+5).empty(); 
$("#datanote"+(start+3)+"-"+1).empty();
$("#datanote"+(start+3)+"-"+2).empty();   
$("#datanote"+(start+3)+"-"+3).empty();   
$("#datanote"+(start+3)+"-"+4).empty();   
$("#datanote"+(start+3)+"-"+5).empty(); 
}
}

$(document).ready(function() { 
$("#Warm").hide();
$("#Aircleaner").hide();      
showCanva2(23,"WarmCanva1");
showCanva2(21,"WarmCanva2");
showCanva2(28,"WarmCanva3");
showCanva3(617,"airCanva1");
showCanva(75,"airCanva2");
showCanva2(8,"airCanva3");
showCanva2(4,"airCanva4");
   
         $.ajax({
            type: "GET",
            url: "/bsverification"
          }).done(function(string){
          $.each(JSON.parse(string),function(i,item){
            var objd1=new mydatanote(item.d1_1,item.d1_2,item.d1_3,item.d1_4,1,item.d1_t);
            var objd2=new mydatanote(item.d2_1,item.d2_2,item.d2_3,item.d2_4,5,item.d2_t);
            var objd3=new mydatanote(item.d3_1,item.d3_2,item.d3_3,item.d3_4,9,item.d3_t);
            var objd4=new mydatanote(item.d4_1,item.d4_2,item.d4_3,item.d4_4,13,item.d4_t);
            if(item.d1_1!=null) 
            {
            objd1.show();
            }
            if(item.d2_1!=null) 
            {
            objd2.show();
            }
            if(item.d3_1!=null) 
            {
            objd3.show();
            }
            if(item.d4_1!=null) 
            {
            objd4.show();
            }

          });
          });
$("#Clean").click(function(e) {
$.ajax({
            type: "CLEAN",
            url: "/bsverification",

          });
var objd1=new mydatanote(0,0,0,0,1,0);
var objd2=new mydatanote(0,0,0,0,5,0);
var objd3=new mydatanote(0,0,0,0,9,0);
var objd4=new mydatanote(0,0,0,0,13,0);
objd1.clean();
objd2.clean();
objd3.clean();
objd4.clean();
          
});
$("#Notes").click(function(e) {
var time=new Date();
var now=time.getFullYear()+"-"+(time.getMonth()+1)+"-"+time.getDate()+"&nbsp;&nbsp;"+time.getHours()+":"+time.getMinutes();
var dd1_1=document.getElementById("datanote1-1");
var dd5_1=document.getElementById("datanote5-1");
var dd9_1=document.getElementById("datanote9-1");
var dd13_1=document.getElementById("datanote13-1");
if(dd1_1.innerHTML=="")
{
    var objd1=new mydatanote(d1,d2,d3,d4,1,now);
    objd1.show();
         $.ajax({
            type: "POST",
            url: "/bsverification",
            dataType: "json",
            data:{"id":"1","time": now,"d1":d1,"d2":d2,"d3":d3,"d4":d4}
          });

}
else{
if(dd5_1.innerHTML=="")
{
    var objd1=new mydatanote(d1,d2,d3,d4,5,now);
    objd1.show();
         $.ajax({
            type: "POST",
            url: "/bsverification",
            dataType: "json",
            data:{"id":"2","time": now,"d1":d1,"d2":d2,"d3":d3,"d4":d4}
          });

}
else{
if(dd9_1.innerHTML=="")
{
    var objd1=new mydatanote(d1,d2,d3,d4,9,now);
    objd1.show();
         $.ajax({
            type: "POST",
            url: "/bsverification",
            dataType: "json",
            data:{"id":"3","time": now,"d1":d1,"d2":d2,"d3":d3,"d4":d4}
          });

}
else{
if(dd13_1.innerHTML=="")
{
    var objd1=new mydatanote(d1,d2,d3,d4,13,now);
    objd1.show();
         $.ajax({
            type: "POST",
            url: "/bsverification",
            dataType: "json",
            data:{"id":"4","time": now,"d1":d1,"d2":d2,"d3":d3,"d4":d4}
          });

}
}}}

        });

    $.ajax({
            type: "refresh",
            url: "/bsverification"
          }).done(function(string) {
            $.each(JSON.parse(string), function(i, item){
            d1=item.d1;
            d2=item.d2;
            d3=item.d3;
            d4=item.d4;
            showCanva(parseInt(d1),"myCanva1");
            showCanva(parseInt(d2),"myCanva2");
            showCanva(parseInt(d3),"myCanva3");
            showCanva(parseInt(d4),"myCanva4");
            });
          });
          
        $("#refresh").click(function(e) {
          $.ajax({
            type: "refresh",
            url: "/bsverification"
          })
          .done(function(string) {
            $.each(JSON.parse(string), function(i, item){
            d1=item.d1;
            d2=item.d2;
            d3=item.d3;
            d4=item.d4;
            showCanva(parseInt(d1),"myCanva1");
            showCanva(parseInt(d2),"myCanva2");
            showCanva(parseInt(d3),"myCanva3");
            showCanva(parseInt(d4),"myCanva4");
            });
          });
          e.preventDefault();
        });
        var balheatmode = 0;
        var parheatmode = 0;
        var rooheatmode = 0;
        var Aircleanmode = 0;
        $("#Balheat").click(function(e) {
            balheatmode+=1;
            if(balheatmode==1)
            {$("#Balheat").attr("class", "btn btn-danger btn-circle btn-xl");             
          }
            if(balheatmode==2)
            {
            $("#Balheat").attr("class", "btn btn-inverse btn-circle btn-xl");
            balheatmode=0;
            }
        });
        $("#Parheat").click(function(e) {
               $("#Parheat").attr("class", "btn btn-danger btn-circle btn-xl");   
            parheatmode+=1;
            if(parheatmode==2)
            {
            $("#Parheat").attr("class", "btn btn-inverse btn-circle btn-xl");
            parheatmode=0;
            }               
        });
        $("#Rooheat").click(function(e) {
               $("#Rooheat").attr("class", "btn btn-danger btn-circle btn-xl");     
            rooheatmode+=1;
            if(rooheatmode==2)
            {
            $("#Rooheat").attr("class", "btn btn-inverse btn-circle btn-xl");
            rooheatmode=0;
            }               
        });
    $("#Cleaner").click(function(e) {
            Aircleanmode+=1;
            if(Aircleanmode==1)
            {$("#Cleaner").attr("class", "btn btn-danger btn-circle btn-xl");             
            $.ajax({
            type: "OPEN",
            url: "/bsverification"
          });
          }
            if(Aircleanmode==2)
            {
            $("#Cleaner").attr("class", "btn btn-inverse btn-circle btn-xl");
            Aircleanmode=0;
            $.ajax({
            type: "SHUT",
            url: "/bsverification"
          });
            }
        });     
        $("#Warmchange").click(function(e) {
             $("#MainControl").hide();
             $("#Aircleaner").hide();
             $("#Warm").show();
        });
                $("#Mainchange").click(function(e) {
             $("#MainControl").show();
             $("#Aircleaner").hide();
             $("#Warm").hide();
        });
        $("#Airchange").click(function(e) {
             $("#MainControl").hide();
             $("#Aircleaner").show();
             $("#Warm").hide();
        });
        
        });