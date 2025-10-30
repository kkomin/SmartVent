from flask import Flask, render_template, jsonify
import mysql.connector

app = Flask(__name__)

# DB 연결 설정
db_config = {
    'host': 'localhost',
    'user': 'root',
    'password': '1234',
    'database': 'smart_env_db'
}

# 메인 페이지
@app.route('/')
def dashboard():
    return render_template('dashboard.html')

# 최신 데이터 API
@app.route('/api/latest')
def get_latest_data():
    conn = mysql.connector.connect(**db_config)
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
    conn = mysql.connector.connect(**db_config)
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

if __name__ == '__main__':
    app.run(debug=True)
