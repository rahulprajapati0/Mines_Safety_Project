const db5 = firebase.database();
const display_co2=document.querySelector('#co2_lev');
const ref5 = db5.ref('live_data/'+'gasdata/'+'co2/');
ref5.on('value', snapshot => {
    const data = snapshot.val();
    console.log(data);
    display_co2.innerHTML = data;
})