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

if __name__ == '__main__':
    app.run(debug=True)
