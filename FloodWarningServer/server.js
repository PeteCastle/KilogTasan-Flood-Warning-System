const express = require('express');
const app = express();

app.get('/api/currentDate', (req, res) => {
    const currentDate = new Date();
    const year = currentDate.getFullYear();
    const month = currentDate.getMonth() + 1; // months are 0-indexed
    const day = currentDate.getDate();
    let hour = currentDate.getHours();
    let ampm = 'AM';
    if (hour >= 12) {
      hour -= 12;
      ampm = 'PM';
    }
    if (hour === 0) {
      hour = 12;
    }
    const minute = currentDate.getMinutes();
    const seconds = currentDate.getSeconds();
    const timeZone = currentDate.getTimezoneOffset();
    const dateString = `${year}-${month}-${day} ${hour}:${minute}:${seconds} ${ampm} (GMT${timeZone})`;
    res.send(dateString);
  });

app.listen(3000, () => console.log('Server listening on port 3000'));