const db3 = firebase.database();
const display_o2=document.querySelector('#o2_lev');
const ref3 = db3.ref('live_data/'+'gasdata/'+'o2/');
var oxy;
ref3.on('value', snapshot => {
    const data = snapshot.val();
    console.log(data);
    display_o2.innerHTML = data;
    
})

