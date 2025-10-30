from flask import Flask, render_template, jsonify, request
import mysql.connector
from datetime import datetime

app = Flask(__name__)

# DB 연결 설정
def get_db_connection():
    return mysql.connector.connect(
        host= 'localhost',
        user= 'root',
        password= '1234',
        database= 'smart_env_db'
    )

# 메인 페이지
@app.route('/')
def dashboard():
    return render_template('dashboard.html')

# 최신 데이터 API
@app.route('/api/latest')
def get_latest_data():
    conn = get_db_connection()
    cursor = conn.cursor(dictionary=True)

    cursor.execute("""
        SELECT * FROM environment_data 
        ORDER BY timestamp DESC 
        LIMIT 1
    """)
    data = cursor.fetchone()

    cursor.close()
    conn.close()

    return jsonify(data)

# 그래프용 API 추가
@app.route('/api/history')
def get_history_data():
    conn = get_db_connection()
    cursor = conn.cursor(dictionary=True)

    cursor.execute("""
        SELECT timestamp, tmp_in, hum_in, tmp_out, hum_out
        FROM environment_data
        ORDER BY timestamp DESC
        LIMIT 20
    """)
    rows = cursor.fetchall()

    cursor.close()
    conn.close()

    # 최신순으로 가져왔으니 역순으로 돌려줌
    return jsonify(list(reversed(rows)))

# 수동 환기 제어
@app.route('/api/vent', methods=['POST'])
def update_vent():
    data = request.get_json()
    vent_status = data['vent_status']

    conn = get_db_connection()
    cursor = conn.cursor()
    timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')

    # 최신 센서 데이터 가져와서 vent_status만 수정해 새 row 추가
    cursor.execute(
        "INSERT INTO environment_data (tmp_in, hum_in, tmp_out, hum_out, weather_desc, pm25, pm10, vent_status, timestamp) "
        "SELECT tmp_in, hum_in, tmp_out, hum_out, weather_desc, pm25, pm10, %s, %s FROM environment_data ORDER BY id DESC LIMIT 1",
        (vent_status, timestamp)
    )

    # 로그 기록 추가
    cursor.execute(
        "INSERT INTO system_logs (log_time, message, log_level, related_table, related_id) "
        "VALUES (%s, %s, 'INFO', 'environment_data', LAST_INSERT_ID())",
        (timestamp, f"사용자 수동 환기 {'ON' if vent_status == 1 else 'OFF'}")
    )

    conn.commit()
    cursor.close()
    conn.close()
    return jsonify({"success": True})


if __name__ == '__main__':
    app.run(debug=True)
