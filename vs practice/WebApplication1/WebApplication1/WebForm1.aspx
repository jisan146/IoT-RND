<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="WebForm1.aspx.cs" Inherits="WebApplication1.WebForm1" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title></title>
</head>
<body>
   <div style=" height:400px;overflow:auto">
   <script type="text/javascript">
       var ga, gb;
       function myMap() {
           var a = ga;
           var b = gb;
           var c = 'gdf';


           var myCenter = new google.maps.LatLng(23.7561067, 90.38719609999998);
           var mapCanvas = document.getElementById("dvMap");
           var mapOptions = { center: myCenter, zoom: 18 };
           var map = new google.maps.Map(mapCanvas, mapOptions);
           var marker = new google.maps.Marker({ position: myCenter });
           marker.setMap(map);

           var infowindow = new google.maps.InfoWindow({
               content: c
           });
           infowindow.open(map, marker);
           google.maps.event.addListener(marker, 'click', function () {
               var infowindow = new google.maps.InfoWindow({
                   content: c
               });
               infowindow.open(map, marker);
           });
       }
</script>
   <form id="form1" runat="server">
        <asp:ScriptManager ID="ScriptManager1" runat="server">

       </asp:ScriptManager>
       <div id="dvMap" 
            style="border: 2px solid #CCCCCC; margin: auto; width: 100%; height: 300px"></div>
 <script  src="https://maps.googleapis.com/maps/api/js?key=AIzaSyA7ahws4ZDxBimvHaaihnIH12BPK7ysyCM&callback=initMap"
  type="text/javascript"></script>
      
<script type="text/javascript">

    var x = document.getElementById("demo");

    function getLocation() {
        if (navigator.geolocation) {
            navigator.geolocation.getCurrentPosition(showPosition);
            startTime();
        } else {
            x.innerHTML = "Geolocation is not supported by this browser.";
        }
    }

    function showPosition(position) {
        // x.innerHTML = "Latitude: " + position.coords.latitude +
        // "<br>Longitude: " + position.coords.longitude; window.location = "WebForm1.aspx";
        var latlondata = position.coords.latitude + "," + position.coords.longitude;



      

        ga = position.coords.latitude;
        gb = position.coords.longitude;


    }

</script>
       


     
      
</form>
</div>
</body>
</html>
