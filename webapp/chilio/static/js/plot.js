var ctx = document.getElementById("plotSensor").getContext('2d');
var myChart = new Chart(ctx, {
    type: "scatter",
    data: {
        datasets: [{
            label: "Moisture",
            data: [
                {% for sample in samples %}
                {x: new Date( {{ sample.tmilliseconds }} ), y: {{ sample.value }} },
                {% endfor %}
            ],
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
