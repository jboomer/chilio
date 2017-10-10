var ctx = document.getElementById("plotSensor").getContext('2d');
var myChart = new Chart(ctx, {
    type: "scatter",
    data: {
        datasets: [{
            label: "Moisture",
            data: data,
            fill: false,
            showLine: true,
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
