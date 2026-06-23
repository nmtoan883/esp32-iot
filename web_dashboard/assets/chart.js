const TEMP_WARNING_THRESHOLD = 35.0; // Celsius

// Initialize Chart
const ctx = document.getElementById('sensorChart').getContext('2d');

// Chart configuration with dark theme aesthetics
const gradientTemp = ctx.createLinearGradient(0, 0, 0, 400);
gradientTemp.addColorStop(0, 'rgba(255, 91, 91, 0.5)');
gradientTemp.addColorStop(1, 'rgba(255, 91, 91, 0.0)');

const gradientHum = ctx.createLinearGradient(0, 0, 0, 400);
gradientHum.addColorStop(0, 'rgba(14, 165, 233, 0.5)');
gradientHum.addColorStop(1, 'rgba(14, 165, 233, 0.0)');

const sensorChart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: [],
        datasets: [
            {
                label: 'Temperature (°C)',
                data: [],
                borderColor: '#ff5b5b',
                backgroundColor: gradientTemp,
                borderWidth: 3,
                pointBackgroundColor: '#0b0f19',
                pointBorderColor: '#ff5b5b',
                pointBorderWidth: 2,
                pointRadius: 4,
                pointHoverRadius: 6,
                fill: true,
                tension: 0.4 // Smooth curves
            },
            {
                label: 'Humidity (%)',
                data: [],
                borderColor: '#0ea5e9',
                backgroundColor: gradientHum,
                borderWidth: 3,
                pointBackgroundColor: '#0b0f19',
                pointBorderColor: '#0ea5e9',
                pointBorderWidth: 2,
                pointRadius: 4,
                pointHoverRadius: 6,
                fill: true,
                tension: 0.4
            }
        ]
    },
    options: {
        responsive: true,
        maintainAspectRatio: false,
        interaction: {
            mode: 'index',
            intersect: false,
        },
        plugins: {
            legend: {
                display: false // We built custom legend in HTML
            },
            tooltip: {
                backgroundColor: 'rgba(15, 23, 42, 0.9)',
                titleColor: '#f8fafc',
                bodyColor: '#cbd5e1',
                borderColor: 'rgba(255,255,255,0.1)',
                borderWidth: 1,
                padding: 12,
                boxPadding: 6
            }
        },
        scales: {
            x: {
                grid: {
                    color: 'rgba(255, 255, 255, 0.05)',
                    drawBorder: false
                },
                ticks: {
                    color: '#94a3b8',
                    maxTicksLimit: 10
                }
            },
            y: {
                grid: {
                    color: 'rgba(255, 255, 255, 0.05)',
                    drawBorder: false
                },
                ticks: {
                    color: '#94a3b8'
                },
                suggestedMin: 0,
                suggestedMax: 100
            }
        }
    }
});

// Function to fetch data from PHP API
async function fetchData() {
    try {
        const response = await fetch('api/get_data.php?limit=20');
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        const data = await response.json();
        updateDashboard(data);
    } catch (error) {
        console.error('Error fetching data:', error);
        document.getElementById('connection-status').textContent = 'Connection Error';
        document.getElementById('connection-status').style.color = '#ff5b5b';
        document.querySelector('.pulse-dot').style.backgroundColor = '#ff5b5b';
        document.querySelector('.pulse-dot').style.animation = 'none';
    }
}

// Function to update the UI
function updateDashboard(data) {
    if (!data || data.length === 0) return;

    // Data connection restored
    document.getElementById('connection-status').textContent = 'Live Data Active';
    document.getElementById('connection-status').style.color = '#10b981';
    document.querySelector('.pulse-dot').style.backgroundColor = '#10b981';
    document.querySelector('.pulse-dot').style.animation = 'pulse 1.5s infinite';

    const latest = data[data.length - 1];
    
    // Update metric cards
    document.getElementById('current-temp').textContent = parseFloat(latest.temperature).toFixed(1);
    document.getElementById('current-hum').textContent = parseFloat(latest.humidity).toFixed(1);
    
    // Format timestamp
    const date = new Date(latest.created_at);
    const timeString = date.toLocaleTimeString('en-US', { hour12: false });
    document.getElementById('last-update').textContent = timeString;

    // Handle high temp alert
    const alertBanner = document.getElementById('alert-banner');
    if (parseFloat(latest.temperature) >= TEMP_WARNING_THRESHOLD) {
        alertBanner.classList.remove('hidden');
    } else {
        alertBanner.classList.add('hidden');
    }

    // Update Chart Arrays
    const labels = [];
    const temps = [];
    const hums = [];

    data.forEach(row => {
        const rowDate = new Date(row.created_at);
        labels.push(rowDate.toLocaleTimeString('en-US', { hour12: false, hour: '2-digit', minute:'2-digit', second:'2-digit' }));
        temps.push(row.temperature);
        hums.push(row.humidity);
    });

    sensorChart.data.labels = labels;
    sensorChart.data.datasets[0].data = temps;
    sensorChart.data.datasets[1].data = hums;
    sensorChart.update();
}

// Fetch data immediately on load
fetchData();

// Then poll every 3 seconds
setInterval(fetchData, 3000);
