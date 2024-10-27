CREATE TABLE IF NOT EXISTS signal_data (
    id SERIAL PRIMARY KEY,
    device_id VARCHAR(50),
    signal_strength INTEGER,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
