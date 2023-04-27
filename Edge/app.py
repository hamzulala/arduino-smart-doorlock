import serial
from flask import Flask, render_template, request, redirect, url_for
from datetime import datetime

app = Flask(__name__)

# Set up the serial connection with the Arduino
ser = serial.Serial('/dev/cu.usbmodem101', 9600)

data_log = []

@app.route("/")
def index():
    global data_log
    while ser.inWaiting() > 0:
        line = ser.readline().decode('utf-8').strip()
        timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        data_log.append({'timestamp': timestamp, 'message': line})

    return render_template("index.html", data_log=data_log)

@app.route("/update_uid", methods=["POST"])
def update_uid():
    new_uid = request.form.get("new_uid")
    if new_uid:
        ser.write((new_uid + "\n").encode("utf-8"))
    return redirect(url_for("index"))

if __name__ == "__main__":
    app.run(debug=True, host='0.0.0.0', port=3000)

