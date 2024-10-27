function updateDateTime() {
    const now = new Date();
    document.getElementById('datetime').innerText = now.toLocaleString();
}

window.onload = updateDateTime;

document.getElementById('refreshButton').addEventListener('click', updateDateTime);
