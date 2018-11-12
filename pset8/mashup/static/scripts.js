// Google Map
var map;

// markers for map
// markers are put on a map
// this is a javascript array
var markers = [];

// info window
// an object
var info = new google.maps.InfoWindow();

// execute when the DOM is fully loaded
// this is a short form for $(document).ready({});
$(function() {

    // styles for map
    // https://developers.google.com/maps/documentation/javascript/styling
    // [] is for arrays {} is for objects
    // so this is array of objects
    var styles = [
        // hide Google's labels
        {
            featureType: "all",
            elementType: "labels",
            stylers: [
                {visibility: "off"}
            ]
        },
// the stylers can be an array of one or more objects
        // hide roads
        {
            featureType: "road",
            elementType: "geometry",
            stylers: [
                {visibility: "off"}
            ]
        }

    ];

    // options for map
    // https://developers.google.com/maps/documentation/javascript/reference#MapOptions
    var options = {
        center: {lat: 33.4484, lng: 111.0937}, // Arizona, 
        disableDefaultUI: true,
        mapTypeId: google.maps.MapTypeId.ROADMAP,
        maxZoom: 14,
        panControl: true,
        // notice how styles is put inside options
        styles: styles,
        zoom: 13,
        zoomControl: true
    };

    // get DOM node in which map will be instantiated
    var canvas = $("#map-canvas").get(0);

    // instantiate map
    map = new google.maps.Map(canvas, options);

    // configure UI once Google Map is idle (i.e., loaded)
    google.maps.event.addListenerOnce(map, "idle", configure);

});

/**
 * Adds marker for place to map.
 */
function addMarker(place)
{
     // place is a javascript object contains a lot of information 
     // instantiate a LatLng object, we will use it for marker
    var LL = new google.maps.LatLng(place["latitude"], place["longitude"]);
    
    // icon for the marker
    var image = "http://maps.google.com/mapfiles/kml/pal2/icon31.png";
    
    // instantiate marker
    var marker = new google.maps.Marker({
        position: LL,
        map: map,
        title: place["place_name"] +", "+ place["admin_name1"],
        label: place["place_name"] +", "+ place["admin_name1"],
        icon : image
    });
    
    
    // get articles for place
    // Now we use ajax to request articles for a given place
    // I give it postal code
    // when requesting the server; the first argument is the url; second is
    // the data to be given, the third is a function to recieve the response
    $.getJSON(Flask.url_for("articles"), {geo: place.postal_code}, function(articles) {
        
        // Only display infowindow if articles exist
        if (!$.isEmptyObject(articles))
        {			
            var topArticles = "<ul>";
            for (var i = 0; i < articles.length; i++)
            {
                // according to helpers.py article has title and link
            	topArticles += "<li><a target='_NEW' href='" + articles[i].link
            	+ "'>" + articles[i].title + "</a></li>";
            }
        }
        
        // close the unordered list of articles
		topArticles += "</ul>";
		
		// attach listener to the marker
        google.maps.event.addListener(marker, 'click', function() {
            showInfo(marker, topArticles);
		});
    });
    
    // add marker to the map markers
    markers.push(marker);
}

/**
 * Configures application.
 */
function configure()
{
    // update UI after map has been dragged
    google.maps.event.addListener(map, "dragend", function() {

        // if info window isn't open
        if (!info.getMap || !info.getMap())
        {
            update();
        }
    });

    // update UI after zoom level changes
    google.maps.event.addListener(map, "zoom_changed", function() {
        update();
    });

    // configure typeahead
    $("#q").typeahead({
        highlight: false,
        minLength: 1
    },
    {
        display: function(suggestion) { return null; },
        limit: 10,
        source: search,
        templates: {
            suggestion: Handlebars.compile(
                "<div>" +
                "{{ place_name }}, {{ admin_name1 }}, {{ postal_code }}" +
                "</div>"
            )
        }
    });

    // re-center map after place is selected from drop-down
    $("#q").on("typeahead:selected", function(eventObject, suggestion, name) {

        // set map's center
        map.setCenter({lat: parseFloat(suggestion.latitude), lng: parseFloat(suggestion.longitude)});

        // update UI
        update();
    });

    // hide info window when text box has focus
    $("#q").focus(function(eventData) {
        info.close();
    });

    // re-enable ctrl- and right-clicking (and thus Inspect Element) on Google Map
    // https://chrome.google.com/webstore/detail/allow-right-click/hompjdfbfmmmgflfjdlnkohcplmboaeo?hl=en
    document.addEventListener("contextmenu", function(event) {
        event.returnValue = true; 
        event.stopPropagation && event.stopPropagation(); 
        event.cancelBubble && event.cancelBubble();
    }, true);

    // update UI
    update();

    // give focus to text box
    $("#q").focus();
}

/**
 * Removes markers from map.
 */
function removeMarkers()
{
     // remove all markers from the map
    for (var i = 0, n = markers.length; i < n; i++)
    {
	    markers[i].setMap(null);
    }
}

/**
 * Searches database for typeahead's suggestions.
 */
function search(query, syncResults, asyncResults)
{
    // get places matching query (asynchronously)
    var parameters = {
        q: query
    };
    $.getJSON(Flask.url_for("search"), parameters)
    .done(function(data, textStatus, jqXHR) {
     
        // call typeahead's callback with search results (i.e., places)
        asyncResults(data);
    })
    .fail(function(jqXHR, textStatus, errorThrown) {

        // log error to browser's console
        console.log(errorThrown.toString());

        // call typeahead's callback with no results
        asyncResults([]);
    });
}

/**
 * Shows info window at marker with content.
 */
function showInfo(marker, content)
{
    // start div
    var div = "<div id='info'>";
    if (typeof(content) == "undefined")
    {
        // http://www.ajaxload.info/
        div += "<img alt='loading' src='/static/ajax-loader.gif'/>";
    }
    else
    {
        div += content;
    }

    // end div
    div += "</div>";

    // set info window's content
    info.setContent(div);

    // open info window (if not already open)
    info.open(map, marker);
}

/**
 * Updates UI's markers.
 */
function update() 
{
    // get map's bounds
    var bounds = map.getBounds();
    var ne = bounds.getNorthEast();
    var sw = bounds.getSouthWest();

    // get places within bounds (asynchronously)
    var parameters = {
        ne: ne.lat() + "," + ne.lng(),
        q: $("#q").val(),
        sw: sw.lat() + "," + sw.lng()
    };
    $.getJSON(Flask.url_for("update"), parameters)
    .done(function(data, textStatus, jqXHR) {

       // remove old markers from map
       removeMarkers();

       // add new markers to map
       for (var i = 0; i < data.length; i++)
       {
           addMarker(data[i]);
       }
    })
    .fail(function(jqXHR, textStatus, errorThrown) {

        // log error to browser's console
        console.log(errorThrown.toString());
    });
};
