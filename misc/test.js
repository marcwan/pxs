
var x = 6;


function yy() {

    var y = 10;

    setTimeout(function () {
        var z = 15;

        console.log(x + " " +y+ " " + z);
    }, 3000);

    y = 12;

    function zz() {
        y = 13
    }

    zz();

    console.log(x + " " +y);
}



yy();
    
