var ctx = document.getElementById("plotSensor").getContext('2d');
var $upbtn = $('#btnUpdate');

$upbtn.click(function() {
    $.getJSON('_api/sensorvals/?sensorid=0', function (samples) {
        update_data(samples)
    });

});

var myChart = new Chart(ctx, {
    type: "scatter",
    data: {
        datasets: [{
            label: "Moisture",
            data: data,
            borderColor: "rgba(0,100,220,1)",           // Color of line between dots
            pointBackgroundColor: "rgba(0,100,220,1)",  // Color of dots
            fill: false,
            showLine: true,
            lineTension: 0.3,
        }]
    },
    options: {
        scales: {
            xAxes: [{
                type: "time",
                display: true,
                time: {
                    unit: 'second'
                }
            }]

        }
    }
});

function update_data(samples) {
    var data = [];
    samples.forEach(
        function (elem, index) {
            var datapoint = 
                {x: new Date(elem.TIME * 1000.0), y: elem.VALUE};
            data.push(datapoint);
    });


    myChart.data.datasets[0].data = data;
    myChart.update();
}
