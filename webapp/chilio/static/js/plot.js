function plotdata(canvas, mydata) {
    var ctx = canvas.getContext('2d');
    var myChart = new Chart(ctx, {
        type: "scatter",
        data: {
            datasets: [{
                label: "Moisture",
                data: mydata,
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
}
