const db2 = firebase.database();
const display_ch4=document.querySelector('#ch4_lev');
const ref2 = db2.ref('live_data/'+'gasdata/'+'ch4/');
var meth;
ref2.on('value', snapshot => {
    const data = snapshot.val();
    console.log(data);
    display_ch4.innerHTML = data;
    
})


