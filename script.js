const db = firebase.database();
const display_demo = document.querySelector('#displayVal');

const ref = db.ref('live_data/'+'dust');
ref.on('value', snapshot => {
    const data = snapshot.val();
    console.log(data);
    display_demo.innerHTML = data;
})
