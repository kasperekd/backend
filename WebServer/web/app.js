var map = L.map('map').setView([55.0, 82.0], 5);

L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    maxZoom: 19,
}).addTo(map);

function fetchBaseStation(cellId) {
    fetch('/api/base-stations/' + cellId)
        .then(response => response.json())
        .then(data => {
            if (data.error) {
                alert("Ошибка: " + data.error);
                return;
            }

            var latitude = data.latitude;
            var longitude = data.longitude;

            if (latitude && longitude) {
                L.marker([latitude, longitude])
                    .addTo(map)
                    .bindPopup(`Cell ID: ${data.cell_id}`);

                map.setView([latitude, longitude], 10);
            } else {
                alert("Данные для Cell ID не найдены");
            }
        })
        .catch(err => {
            console.error("Error fetching base station data:", err);
            alert("Произошла ошибка при получении данных.");
        });
}

document.getElementById('fetchButton').addEventListener('click', function () {
    var cellIdInput = document.getElementById('cellIdInput');
    var cellId = cellIdInput.value.trim();

    if (!cellId) {
        alert("Введите корректный Cell ID");
        return;
    }

    fetchBaseStation(cellId);
});

function fetchAllBaseStations() {
    fetch('/api/base-stations/multiple', {
        method: 'GET',
        headers: {
            'Content-Type': 'application/json'
        }
    })
        .then(response => response.json())
        .then(data => {
            if (data && Array.isArray(data)) {
                data.forEach(item => {
                    if (item.latitude && item.longitude) {
                        L.marker([item.latitude, item.longitude])
                            .addTo(map)
                            .bindPopup(`Cell ID: ${item.cell_id}`);
                    }
                });
            } else {
                alert("Ошибка загрузки данных.");
            }
        })
        .catch(err => {
            console.error("Ошибка при загрузке данных базовых станций:", err);
            alert("Произошла ошибка при получении данных.");
        });
}

document.getElementById('fetchAllButton').addEventListener('click', function () {
    fetchAllBaseStations();
});