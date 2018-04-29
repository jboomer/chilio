
function get_update_data_function(chartObject, sensorid) {

    var ret = function update_data() {
        $.getJSON('_api/sensorvals/?sensorid=' + sensorid, fill_data);
    }

    function fill_data(samples) {
        var data = [];
        samples.forEach(
            function (elem, index) {
                var datapoint = 
                    {x: new Date(elem.TIME * 1000.0), y: elem.VALUE};
                data.push(datapoint);
        });


        chartObject.data.datasets[0].data = data;
        chartObject.update();
    }
    
    return ret;
}


$(function () {
    var ctx = document.getElementById("plotSensor").getContext('2d');
    
    var myChart = new Chart(ctx, {
        type: "scatter",
        data: {
            datasets: [{
                label: "Moisture",
                data: [],
                borderColor: "rgba(0,100,220,1)",  // Color of line between dots
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

    var updateFunc = get_update_data_function(myChart, 0);

    $('#btnUpdate').click(updateFunc);

    $('#btnUpdate').click();

});
