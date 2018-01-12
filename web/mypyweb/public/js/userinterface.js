       function showCheck(a){
            var c = document.getElementById("myCanvas");
          var ctx = c.getContext("2d");
            ctx.clearRect(0,0,1000,1000);
            ctx.font = "80px Arial";
            ctx.fillStyle="white";
            ctx.fillText(a,55,100);
        }
        
        function clean1(value){
            value=value.replace(/[^\w\.\/]/ig,'');
            document.getElementById("limit1").style.display="inline";
            return value;
        }
        
        function limit1none(){
        document.getElementById("limit1").style.display="none";
        }
        
        function clean2(value){
            value=value.replace(/[^\w\.\/]/ig,'');
            document.getElementById("limit2").style.display="inline";
            return value;
        }
        
        function limit2none(){
        document.getElementById("limit2").style.display="none";
        }
        
        function CookieVerification() {
        if($.cookie("login_cookie")==null){
        
        }
        else{
        window.location.href='/backstage';
        }
        }
        
        function CookieCreate(cookie){       
        if (document.getElementById("CookieCheck").checked){ 
        var login_cookie=cookie.substring(1);
        $.cookie('login_cookie', login_cookie, { expires: 7, path: '/' });    
        }
        }
        
      $(document).ready(function() {
        CookieVerification();
        $.ajax({
            type: "GET",
            url: "/verification"
          })
          .done(function(string) {
            showCheck(string);
          });
        
        
        $("#myCanvas").click(function(e) {
          $.ajax({
            type: "GET",
            url: "/verification"
          })
          .done(function(string) {
            showCheck(string);
          });
          e.preventDefault();
        });
        
        $("#signin").click(function(e) {
            var mode ='';
            var cookie ='';
         $.ajax({
            type: "POST",
            url: "/verification",
            dataType: "json",
            data:{"username": $("input[name='username']").val(),"password": $("input[name='password']").val(),"captcha": $("input[name='captcha']").val()}
          })
           .done(function(string) {
            $.each(string, function(i, item){
            mode=item.mode;
            cookie=item.cookie;
            });
            if (mode=='wrong'){
            alert("Captcha text is incorrect ,please try again");
            window.location.reload();
            }
            if (mode=='deny'){
            alert(" Account does not exist or password is incorrect ,please try again ");
            window.location.reload();
            }
            if (mode=='success'){           
            CookieCreate(cookie);
            window.location.href='/backstage';
            }   
          });
          e.preventDefault();
        });      
      });