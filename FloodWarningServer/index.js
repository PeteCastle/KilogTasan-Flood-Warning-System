const express = require('express');
const app = express();

const port = process.env.PORT || 1337;

app.get('/', (req, res) => {

  res.send('HELLO WORLD');
});

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

app.listen(port, () => console.log(`Server listening on port ${port}`));