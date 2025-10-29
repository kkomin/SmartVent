from flask import Flask, render_template
import mysql.connector

app = Flask(__name__)

# DB 연결 함수
def get_latest_environment_data():
    conn = mysql.connector.connect(
        host="localhost",
        user="root",
        password="1234",
        database="smart_env_db"
    )
    cursor = conn.cursor(dictionary=True)
    cursor.execute("SELECT * FROM environment_data ORDER BY timestamp DESC LIMIT 1")
    data = cursor.fetchone()
    cursor.close()
    conn.close()
    return data

# 메인 페이지
@app.route('/')
def index():
    data = get_latest_environment_data()
    return render_template('index.html', data=data)

if __name__ == '__main__':
    app.run(debug=True)
