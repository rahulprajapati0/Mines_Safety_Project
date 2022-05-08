const db_dust = firebase.database();
const db_illumination=firebase.database();
const display_dust=document.querySelector('#dust');
const ref_dust = db_dust.ref('live_data/'+'dust/');
const display_illumination=document.querySelector('#illumination');
const ref_illumination= db_illumination.ref('live_data/'+'illumination/');
ref_dust.on('value', snapshot => {
    const data = snapshot.val();
    console.log(data);
    display_dust.innerHTML = data;
    
})
ref_illumination.on('value', snapshot => {
    const data = snapshot.val();
    console.log(data);
    display_illumination.innerHTML = data;
    
})

