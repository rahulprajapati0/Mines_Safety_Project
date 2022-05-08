const db4 = firebase.database();
const display_co=document.querySelector('#co_lev');
const ref4 = db4.ref('live_data/'+'gasdata/'+'co/');
var mono;
ref4.on('value', snapshot => {
    const data = snapshot.val();
    console.log(data);
    display_co.innerHTML = data;
    
})

